//===================================================================
// min, max, diff (OpenCL)
//
// 04minmaxClに差分を追加,Global Wrok item size==Local Wrok item size
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <limits.h>
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

template<class T> static T min(T a, T b);
template<class T> static T max(T a, T b);

static const char *clProc="clCode";

//-------------------------------------------------------------------
// a, b の小さい方を返す
template<class T>
static T
min(T a, T b)
{
    return ( a < b ) ? a : b;
}

//-------------------------------------------------------------------
// a, b の大きい方を返す
template<class T>
static T
max(T a, T b)
{
    return ( a > b ) ? a : b;
}

//-------------------------------------------------------------------
// C++で処理
static void
checkByCpp(int a[], int size)
{
    int minData=INT_MAX, maxData=INT_MIN;

    for(int i=0; i<size; i++)
    {
        minData=min(minData, a[i]);
        maxData=max(maxData, a[i]);
    }
    int diffData=maxData-minData;

    // print
    printf("C++:    max=%d, min=%d, diff=%d\n",
                        maxData, minData, diffData);
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    try
    {
        cl_int status;

        if(argc!=2)
            throw Cexp( "引数に<*.cl>ファイルを指定してください.");

        int a[100], minData=INT_MAX, maxData=INT_MIN, diffData;
        size_t numOfTotal=sizeof(a)/sizeof(a[0]);

        for(size_t i=0; i<numOfTotal; i++)          // initialize array
            a[i]=(int)rand();


        ClUtils clUtils;

        clUtils.preloadProgram(argv[1]);
        cl_context context    =clUtils.getContext();
        cl_command_queue queue=clUtils.getCmdQueue();

        cl_kernel kernel=clUtils.createKernel(clProc);

                                                    // mem obj(in)
        cl_mem memA=clUtils.createInBuffer(sizeof(cl_int)*numOfTotal, a);

                                                    // mem obj(min)
        cl_mem memMin=clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                            sizeof(cl_int), (void*)&minData, &status);
        if(status!=CL_SUCCESS)
            throw Cexp("clCreateBuffer failed.");

                                                    // mem obj(max)
        cl_mem memMax=clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                            sizeof(cl_int), (void*)&maxData, &status);
        if(status!=CL_SUCCESS)
            throw Cexp("clCreateBuffer failed.");

                                                    // mem obj(diff)
        cl_mem memDiff=clUtils.createOutBuffer(sizeof(cl_int));

                                                    // set kernel parameters
        status =clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
        status+=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memMin);
        status+=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memMax);
        status+=clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&memDiff);
        if(status!=0)
            throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
        size_t workSize[]={numOfTotal};
        status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                workSize, workSize, 0, NULL, NULL);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
        clUtils.enqueueReadBuffer(memMin,  sizeof(cl_int), &minData);
        clUtils.enqueueReadBuffer(memMax,  sizeof(cl_int), &maxData);
        clUtils.enqueueReadBuffer(memDiff, sizeof(cl_int), &diffData);

        clReleaseMemObject(memDiff);
        clReleaseMemObject(memMax);
        clReleaseMemObject(memMin);
        clReleaseMemObject(memA);

                                                    // print
        printf("OpenCL: max=%d, min=%d, diff=%d\n",
                            maxData, minData, diffData);

        checkByCpp(a, numOfTotal);
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
