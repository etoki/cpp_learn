// g++ t_add.cpp ClUtils.cpp -lOpenCL
// ./a.out k_add.cl

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include <stdio.h>
#include "Cexp.h"
#include "ClUtils.h"

#define MAX_SRC_SIZE (0x100000)
static const int CL_SOURCE_SIZE=10000;
static const char *clProc = "add"; //カーネル関数の名前

//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    cl_int status;

    //コンストラクタで処理
    //1,get platform id  2,get device id
    //3,create Context   4,create Command Queue
    ClUtils clUtils;

    float A=1.1f, B=2.2f, C;

    //preloadProgramで処理、カーネルファイル読み込み
    //6,create program object 7,build program
    clUtils.preloadProgram(argv[1]);

    cl_command_queue queue = clUtils.getCmdQueue();

    //5, create memory object
    cl_mem memA=clUtils.createInBuffer(sizeof(A), &A);
    cl_mem memB=clUtils.createInBuffer(sizeof(B), &B);
    cl_mem memC=clUtils.createOutBuffer(sizeof(C));

    //8, create kernel: カーネル関数の名前
    cl_kernel kernel=clUtils.createKernel(clProc);

    //9, set kernel parameters
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    status+= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    status+= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);

    //10, request execute kernel: input command queue
    status=clEnqueueTask(queue, kernel, 0, NULL, NULL);

    //11, obtain results
    clUtils.enqueueReadBuffer(memC, sizeof(C), &C);

    // list results
    printf("%f + %f = %f\n", A, B, C);

    //12, release resources
    clReleaseMemObject(memC);
    clReleaseMemObject(memB);
    clReleaseMemObject(memA);

    return 0;
}
