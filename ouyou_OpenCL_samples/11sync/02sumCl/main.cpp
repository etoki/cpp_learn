//===================================================================
// sum, reduction (OpenCL)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include "../../Cexp/Cexp.h"
#include "../../Cexp/ClUtils.h"

static const char *clProc="clCode";

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

        int a[10000], sum;
        size_t numOfTotal=sizeof(a)/sizeof(a[0]);

        for(size_t i=0; i<numOfTotal; i++)          // initialize array
            a[i]=i;
        sum=0;

        ClUtils clUtils;

        clUtils.preloadProgram(argv[1]);
        cl_context context=clUtils.getContext();
        cl_command_queue queue=clUtils.getCmdQueue();
        cl_kernel kernel=clUtils.createKernel(clProc);

                                                    // mem obj(in)
        cl_mem memA=clUtils.createInBuffer(sizeof(cl_int)*numOfTotal, a);

                                                    // mem obj(out)
        //cl_mem memSum=clUtils.createOutBuffer(sizeof(cl_int));        @
        cl_mem memSum=clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR,
                                            sizeof(cl_int), (void*)&sum, &status);
        if(status!=CL_SUCCESS)
            throw Cexp("clCreateBuffer failed.");

                                                    // set kernel parameters
        status =clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
        status+=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memSum);
        if(status!=0)
            throw Cexp("clSetKernelArg failed.");

                                                    // request execute kernel
        size_t globalSize[]={numOfTotal};
        status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                            globalSize, 0, 0, NULL, NULL);
        if(status!=CL_SUCCESS)
            throw Cexp("clEnqueueNDRangeKernel failed.");

                                                    // get results
        clUtils.enqueueReadBuffer(memSum, sizeof(cl_int), &sum);

        clReleaseMemObject(memSum);
        clReleaseMemObject(memA);

        printf("sum=%d\n", sum);                    // print
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
