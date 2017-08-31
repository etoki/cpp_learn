//===================================================================
// rotate, Bilinear Resize (OpenCL)
//
// rowPitchを渡さず、kernel側で計算する。
//
// $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp ../../Cexp/ClUtils.cpp ../../Cexp/Cbmp.cpp -lOpenCL
// $ ./a.out ~/OpenCLimage/tstImg/Lenna.bmp a.bmp Release/Bilinear.cl 33.123
// ビットマップサイズ= 256 x 256, 24/pixel
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

#ifndef M_PI
#define M_PI 3.14159265358979f              // pi
#endif //M_PI

#define radian2degree(a) (((a)/M_PI)*180.0) // radian to degree
#define degree2radian(a) (((a)/180.0)*M_PI) // degree to radian

static const char *clProc="affineTrans";

//-------------------------------------------------------------------
// calc new size
static void
calcSize(const Cbmp *bmp, const float degree, int *width, int *height)
{
    float tDegree=degree;

    if(tDegree>180.0f)
        tDegree-=180.0f;
    if(tDegree>90.0f)
        tDegree=180.0f-tDegree;

    float radian=(float)degree2radian(tDegree);

    *width = (int)((float)bmp->mPdib->biWidth * cos(radian)
                            + (float)bmp->mAbsHeight*sin(radian));
    *height = (int)((float)bmp->mPdib->biWidth * sin(radian)
                            + (float)bmp->mAbsHeight*cos(radian));
}

//-------------------------------------------------------------------
// rotate(OpenCL)
static void
affineTransCL(const char* clName, Cbmp bmp[], const float inDegree)
{
    cl_int status;
    ClUtils clUtils;
    int outWidth, outHeigth;

    float degree=bmp[0].getHeight()<0 ? inDegree : 360.0f-inDegree;
    calcSize(&bmp[0], degree, &outWidth, &outHeigth);
    int biHeight=bmp[0].getHeight()<0 ? -outHeigth : outHeigth; //y-のための考慮
    bmp[1].create24Dib(outWidth, biHeight);                     //y軸は保存

    clUtils.preloadProgram(clName);
    cl_command_queue queue=clUtils.getCmdQueue();
    cl_kernel kernel=clUtils.createKernel(clProc);

                                                    // mem obj(in)
    cl_mem in=clUtils.createInBuffer(bmp[0].mImageSize, bmp[0].mPbitmap);

                                                    // mem obj(out)
    cl_mem out=clUtils.createOutBuffer(bmp[1].mImageSize, 0xFF);

                                                    // set kernel parameters
    status =clSetKernelArg(kernel, 0, sizeof(cl_int),   (void *)&bmp[0].mPdib->biWidth);
    status =clSetKernelArg(kernel, 1, sizeof(cl_int),   (void *)&bmp[1].mPdib->biWidth);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_int),   (void *)&bmp[0].mAbsHeight);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_int),   (void *)&bmp[1].mAbsHeight);
    status+=clSetKernelArg(kernel, 4, sizeof(cl_int),   (void *)&bmp[0].mPixelPitch);
    status+=clSetKernelArg(kernel, 5, sizeof(cl_int),   (void *)&bmp[1].mPixelPitch);
    status+=clSetKernelArg(kernel, 6, sizeof(cl_float), (void *)&degree);
    status+=clSetKernelArg(kernel, 7, sizeof(cl_mem),   (void *)&in);
    status+=clSetKernelArg(kernel, 8, sizeof(cl_mem),   (void *)&out);
    if(status!=0)
        throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
    size_t globalSize[]={bmp[1].mPdib->biWidth, bmp[1].mAbsHeight};
    status=clEnqueueNDRangeKernel(queue, kernel, 2, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
    clUtils.enqueueReadBuffer(out, bmp[1].mImageSize, bmp[1].mPbitmap);

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
        if(degree>360.0f || degree <0.0f)
            throw Cexp( "角度は0°～360°で指定してください.");

        Cbmp bmp[2];
        bmp[0].loadFromFile(argv[1]);               // load bitmap

        if(bmp[0].getBitsPerPixcel()!=24 && bmp[0].getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp[0].getWidth(), bmp[0].getHeight(), bmp[0].getBitsPerPixcel());

        affineTransCL(argv[3], bmp, degree);        // rotate

        bmp[1].saveToFile(argv[2]);                 // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
