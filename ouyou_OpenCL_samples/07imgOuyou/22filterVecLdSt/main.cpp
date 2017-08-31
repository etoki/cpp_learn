//===================================================================
// filter program, vload3/4, vstore3/4を使用する。
// 24bpp/32bppを分けて記述、24bppはvload3、vstore3の
// offsetの関係で4ピクセルの整数倍でなければならない。
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

static const char *clProc="clCode";


//-------------------------------------------------------------------
// filter(OpenCL)
static void
filterCL(const char* clName, const Cbmp* bmp)
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
    status =clSetKernelArg(kernel, 0, sizeof(cl_int), (void *)&bmp->mPdib->biWidth);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_int), (void *)&bmp->mPixelPitch);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&in);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&out);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
    size_t globalSize[]={bmp->getWidth()-2, bmp->getAbsHeight()-2};
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
        if(argc!=4)
            throw Cexp( "引数に以下の値を指定してください.\n" \
                        "  <入力ファイル>  <出力ファイル>  <*.cl>");

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getWidth()%4!=0 && bmp.getBitsPerPixcel()==24)
            throw Cexp("bmp not 24bpp && 4pix. boundary, nor 32bpp.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        filterCL(argv[3], &bmp);                    // filter

        bmp.saveToFile(argv[2]);                    // save bitmap file
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
