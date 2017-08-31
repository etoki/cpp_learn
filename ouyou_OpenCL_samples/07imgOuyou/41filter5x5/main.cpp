//===================================================================
// big oprator (OpenCL)
//
// XRGBには対応しない。
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

#pragma pack(push, 1)

typedef struct
{
    int   height;
    int   width;
    int   rowPitch;
    int   pixelPitch;
} paraType;

#pragma pack(pop)

static const char* clProc="clCode";


//-------------------------------------------------------------------
// search
static void
filterCL(const char* clName, const Cbmp* bmp)
{
    cl_int status;
    ClUtils clUtils;
    paraType param;

    float filterOp[][5]={

    { 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f },

    { 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f },

    { 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f },

    { 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f },

    { 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f, 1.0f/25.0f }

    };

    param.height    =bmp->mAbsHeight;               // set param
    param.width     =bmp->mPdib->biWidth;
    param.rowPitch  =bmp->mRowPitch;
    param.pixelPitch=bmp->mPixelPitch;

    clUtils.preloadProgram(clName);
    cl_command_queue queue=clUtils.getCmdQueue();
    cl_kernel kernel=clUtils.createKernel(clProc);

                                                    // mem obj(param)
    cl_mem memParam=clUtils.createInBuffer(sizeof(paraType), &param);

                                                    // mem obj(op)
    cl_mem op=clUtils.createInBuffer(sizeof(filterOp), &filterOp);

                                                    // mem obj(in)
    cl_mem in=clUtils.createInBuffer(bmp->mImageSize, bmp->mPbitmap);

                                                    // mem obj(out)
    cl_mem out=clUtils.createOutBuffer(bmp->mImageSize, 0);

    status =clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memParam);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&op);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&in);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
    size_t globalSize[]={param.width-(sizeof(filterOp[0])/sizeof(filterOp[0][0]))+1,
                         param.height-(sizeof(filterOp)/sizeof(filterOp[0]))+1};
    status=clEnqueueNDRangeKernel(queue, kernel, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
    clUtils.enqueueReadBuffer(out, bmp->mImageSize, bmp->mPbitmap);

    clReleaseMemObject(out);
    clReleaseMemObject(in);
    clReleaseMemObject(op);
    clReleaseMemObject(memParam);
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
                        "  <入力ファイル>  <出力ファイル>  <*.clファイル>");

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "画像ファイル= %d x %d, %d/pixel\n",
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
