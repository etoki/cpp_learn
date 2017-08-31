//===================================================================
// min, max (OpenCL)
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

static const char *clMinProc="minCl";
static const char *clMaxProc="maxCl";

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

    // print
    printf("C++:    max=%d, min=%d\n", maxData, minData);
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

        int a[100000], minData=INT_MAX, maxData=INT_MIN;
        size_t numOfTotal=sizeof(a)/sizeof(a[0]);

        for(size_t i=0; i<numOfTotal; i++)          // initialize array
            a[i]=(int)rand();


        ClUtils clUtils;

        clUtils.preloadProgram(argv[1]);
        cl_context context=clUtils.getContext();
        cl_command_queue queue=clUtils.getOutOfOrderQueue();

        cl_kernel kernelMin=clUtils.createKernel(clMinProc);
        cl_kernel KernelMax=clUtils.createAddtionalKernel(clMaxProc);

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

                                                    // set kernel parameters
        status =clSetKernelArg(kernelMin, 0, sizeof(cl_mem),  (void *)&memA);
        status+=clSetKernelArg(kernelMin, 1, sizeof(cl_mem),  (void *)&memMin);
        status+=clSetKernelArg(kernelMin, 2, sizeof(cl_uint), (void *)&numOfTotal);

        status+=clSetKernelArg(KernelMax, 0, sizeof(cl_mem),  (void *)&memA);
        status+=clSetKernelArg(KernelMax, 1, sizeof(cl_mem),  (void *)&memMax);
        status+=clSetKernelArg(KernelMax, 2, sizeof(cl_uint), (void *)&numOfTotal);
        if(status!=0)
            throw Cexp("clSetKernelArg failed.");


                                                    // request execute kernel
        cl_event eventKernel[2];
        status=clEnqueueTask(queue, kernelMin, 0, NULL, &eventKernel[0]);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueTask failed.");

        printf("        enqueued clEnqueueTask API(min).\n");


                                                    // request execute kernel
        status=clEnqueueTask(queue, KernelMax, 0, NULL, &eventKernel[1]);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueTask failed.");

        printf("        enqueued clEnqueueTask API(max).\n");


                                                    // get results
        cl_event eventRead[2];
        status=clEnqueueReadBuffer(queue, memMin, CL_FALSE, 0,
                        sizeof(cl_int), &minData,
                                1, &eventKernel[0], &eventRead[0]);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueReadBuffer failed.");

        printf("        enqueued clEnqueueReadBuffer API(min).\n");


        status=clEnqueueReadBuffer(queue, memMax, CL_FALSE, 0,
                        sizeof(cl_int), &maxData,
                                1, &eventKernel[1], &eventRead[1]);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueReadBuffer failed.");

        printf("        enqueued clEnqueueReadBuffer API(max).\n");


        status=clWaitForEvents(2, eventRead);
        if(status!=CL_SUCCESS)
            throw Cexp("clWaitForEvents failed.");

        printf("        clEnqueueReadBuffer event cleared.\n\n");


        clReleaseMemObject(memMax);
        clReleaseMemObject(memMin);
        clReleaseMemObject(memA);
        clReleaseKernel(KernelMax);

                                                    // print
        printf("OpenCL: max=%d, min=%d\n", maxData, minData);

        checkByCpp(a, numOfTotal);
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
