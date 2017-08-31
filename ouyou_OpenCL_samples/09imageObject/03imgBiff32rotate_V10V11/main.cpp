//===================================================================
// 21rotateClをimageBufferへ(OpenCL 1.0-1.1)
//
// 32 bpp BGRAのみを対象とする、フォーマットチェックは行っていない。
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
//#define CL_USE_DEPRECATED_OPENCL_1_0_APIS   // use OpenCL 1.0 APIs
//#define CL_USE_DEPRECATED_OPENCL_1_1_APIS   // use OpenCL 1.1 APIs

#include <stdio.h>
#include <stdlib.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

static const char *clProc="affineTrans";

//-------------------------------------------------------------------
// rotate(OpenCL)
static void
affineTransCL(const char* clName, const Cbmp* bmp, const float inDegree)
{
    cl_int status;
    ClUtils clUtils;

    float degree=bmp->getHeight()<0 ? inDegree : 360.0f-inDegree;

    int width=bmp->getWidth();
    int height=bmp->getAbsHeight();

    clUtils.preloadProgram(clName);

    cl_context       context=clUtils.getContext();
    cl_command_queue queue  =clUtils.getCmdQueue();
    cl_kernel kernel=clUtils.createKernel(clProc);

    cl_image_format format;                         // set image format
    format.image_channel_data_type=CL_UNORM_INT8;
    format.image_channel_order    =CL_BGRA;

    cl_mem in=clCreateImage2D(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            &format, width, height, 0, bmp->mPbitmap, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateImage2D failed.");

    cl_mem out=clCreateImage2D(context, CL_MEM_WRITE_ONLY, &format,
                                    width, height, 0, NULL, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateImage2D failed.");

    status =clSetKernelArg(kernel, 0, sizeof(cl_mem),   (void *)&in);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_mem),   (void *)&out);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_float), (void *)&degree);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

    size_t globalSize[]={width, height};            // request execute kernel
    status=clEnqueueNDRangeKernel(queue, kernel, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

    const size_t origin[]={0, 0, 0};                // get results
    const size_t region[]={width, height, 1};
    status=clEnqueueReadImage(queue, out, CL_TRUE,  // get results
                origin, region, 0, 0, bmp->mPbitmap, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueReadImage failed.");

    clReleaseMemObject(out);
    clReleaseMemObject(in);
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    try
    {
        if(argc!=5)
            throw Cexp( "引数に以下の値を指定してください.\n" \
                        "  <入力ファイル>  <出力ファイル>  <*.clファイル>  <回転角度>");

        float degree=(float)atof(argv[4]);

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        affineTransCL(argv[3], &bmp, degree);       // rotate

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
