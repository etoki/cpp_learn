//===================================================================
// 05filterBasic/01filterをimageBufferへ(OpenCL 1.2)
//
// 32 bpp BGRAのみを対象とする、フォーマットチェックは行っていない。
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <string.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

static const char *clGS="grayscale";
static const char *clFltr="filter";

//-------------------------------------------------------------------
// filter(OpenCL)
static void
filterCL(const char* clName, const Cbmp* bmp)
{
    cl_int status;
    ClUtils clUtils;

    int width=bmp->getWidth();
    int height=bmp->getAbsHeight();

    clUtils.preloadProgram(clName);

    cl_context       context=clUtils.getContext();
    cl_command_queue queue  =clUtils.getCmdQueue();
    cl_kernel kGs=clUtils.createKernel(clGS);
    cl_kernel kFltr=clUtils.createAddtionalKernel(clFltr);

    cl_image_format format;                         // set image format
    format.image_channel_data_type=CL_UNORM_INT8;
    format.image_channel_order    =CL_BGRA;

    cl_image_desc idesc;
    memset(&idesc, 0, sizeof(cl_image_desc));
    idesc.image_type=CL_MEM_OBJECT_IMAGE2D;
    idesc.image_width=width;
    idesc.image_height=height;
    cl_mem memRgb=clCreateImage(context, CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                                &format, &idesc, bmp->mPbitmap, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateImage failed.");

    cl_mem memGs=clCreateImage(context, CL_MEM_READ_WRITE,
                                    &format, &idesc, NULL, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateImage failed.");

    status =clSetKernelArg(kGs, 0, sizeof(cl_mem), (void *)&memRgb);
    status+=clSetKernelArg(kGs, 1, sizeof(cl_mem), (void *)&memGs);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

    size_t globalSize[]={width, height};            // request gray scale kernel
    status=clEnqueueNDRangeKernel(queue, kGs, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

    const size_t origin[]={0, 0, 0};                // request fill buffer
    const size_t region[]={width, height, 1};
    cl_float4 fillColor={0,0,0,0xFF};
    status=clEnqueueFillImage (queue, memRgb,(void*)&fillColor,
                                        origin,region,0, NULL,NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueFillImage failed.");


    status =clSetKernelArg(kFltr, 0, sizeof(cl_mem), (void *)&memGs);
    status+=clSetKernelArg(kFltr, 1, sizeof(cl_mem), (void *)&memRgb);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

    globalSize[0]-=2;                               // request filter kernel
    globalSize[1]-=2;
    status=clEnqueueNDRangeKernel(queue, kFltr, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");


    status=clEnqueueReadImage(queue, memRgb, CL_TRUE,// get results
                origin, region, 0, 0, bmp->mPbitmap, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueReadImage failed.");

    clReleaseMemObject(memRgb);
    clReleaseMemObject(memGs);
    clReleaseKernel(kFltr);
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    try
    {
        if(argc!=4)
            throw Cexp( "引数に以下の値を指定してください.\n" \
                        "  <入力ファイル>  <出力ファイル>  <*.cl>");

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        filterCL(argv[3], &bmp);                    // filer

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
