//===================================================================
// zoom (OpenCL)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

static const char *clProc="affineTrans";

//-------------------------------------------------------------------
// zoom(OpenCL)
static void
affineTransCL(const char* clName, const Cbmp* bmp, const float scale)
{
    cl_int status;
    ClUtils clUtils;

    clUtils.preloadProgram(clName);
    cl_command_queue queue=clUtils.getCmdQueue();
    cl_kernel kernel=clUtils.createKernel(clProc);

                                                    // mem obj(in)
    cl_mem in=clUtils.createInBuffer(bmp->mImageSize, bmp->mPbitmap);

                                                    // mem obj(out)
    cl_mem out=clUtils.createOutBuffer(bmp->mImageSize, 0xFF);

                                                    // set kernel parameters
    status =clSetKernelArg(kernel, 0, sizeof(cl_int),   (void *)&bmp->mPdib->biWidth);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_int),   (void *)&bmp->mAbsHeight);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_int),   (void *)&bmp->mPixelPitch);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_float), (void *)&scale);
    status+=clSetKernelArg(kernel, 4, sizeof(cl_mem),   (void *)&in);
    status+=clSetKernelArg(kernel, 5, sizeof(cl_mem),   (void *)&out);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
    size_t globalSize[]={bmp->mPdib->biWidth, bmp->mAbsHeight};
    status=clEnqueueNDRangeKernel(queue, kernel, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
    clUtils.enqueueReadBuffer(out, bmp->mImageSize, bmp->mPbitmap);

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
                        "  <入力ファイル>  <出力ファイル>  <*.clファイル>  <スケール>");

        float scale=(float)atof(argv[4]);

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        affineTransCL(argv[3], &bmp, scale);        // zoom

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
