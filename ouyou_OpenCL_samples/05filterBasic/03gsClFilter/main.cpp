//===================================================================
// OpenCLでグレイスケール化とフィルタ処理する。二つのカーネル。
// 出力は、元のところを使うので、枠が残る、消したいならfillする
// こと。
//
//
// $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp ../../Cexp/ClUtils.cpp ../../Cexp/Cbmp.cpp -lOpenCL
// $ ./a.out '/home/k/OpenCLimage/tstImg/Parrots.bmp' a.bmp Release/embossMono.cl
// ビットマップサイズ= 256 x 256, 24/pixel
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
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

    cl_context       context=clUtils.getContext();
    cl_command_queue queue  =clUtils.getCmdQueue();

    clUtils.preloadProgram(clName);

    cl_kernel kGs=clUtils.createKernel(clGS);       // create kernels
    cl_kernel kFltr=clUtils.createAddtionalKernel(clFltr);

    unsigned const int gsSize=bmp->getAbsHeight()*bmp->getWidth();

    cl_mem memGs=clCreateBuffer(context,            // create memory object
                        CL_MEM_READ_WRITE, gsSize, NULL, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateBuffer failed.");

    cl_mem memRgb=clCreateBuffer(context,           // create memory object(memRgb)
                        CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR,
                        bmp->mImageSize, bmp->mPbitmap, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateBuffer function failed.");

                                                    // set kernel parameters
    status =clSetKernelArg(kGs, 0, sizeof(cl_int), (void *)&bmp->mPdib->biWidth);
    status+=clSetKernelArg(kGs, 1, sizeof(cl_int), (void *)&bmp->mPixelPitch);
    status+=clSetKernelArg(kGs, 2, sizeof(cl_mem), (void *)&memRgb);
    status+=clSetKernelArg(kGs, 3, sizeof(cl_mem), (void *)&memGs);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // enqueue gray scale kernel
    size_t globalSize[]={bmp->getWidth(),bmp->getAbsHeight()};
    status=clEnqueueNDRangeKernel(queue, kGs, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");


                                                    // set kernel parameters
    status =clSetKernelArg(kFltr, 0, sizeof(cl_uint), (void *)&bmp->mPdib->biWidth);
    status+=clSetKernelArg(kFltr, 1, sizeof(cl_int),  (void *)&bmp->mPixelPitch);
    status+=clSetKernelArg(kFltr, 2, sizeof(memGs),   (void *)&memGs);
    status+=clSetKernelArg(kFltr, 3, sizeof(memRgb),  (void *)&memRgb);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // enqueue filter kernel
    globalSize[0]-=2;
    globalSize[1]-=2;
    status=clEnqueueNDRangeKernel(queue, kFltr, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
    clUtils.enqueueReadBuffer(memRgb, bmp->mImageSize, bmp->mPbitmap);

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

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        filterCL(argv[3], &bmp);                    // filter

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
