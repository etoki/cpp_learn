// g++ work.cpp src/ClUtils.cpp -lOpenCL
// ./a.out kernel/work.cl

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include <stdio.h>
#include "src/Cexp.h"
#include "src/ClUtils.h"

#define MAX_SRC_SIZE (0x100000)
static const int CL_SOURCE_SIZE=10000;
static const char *clProc = "clCode";

//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
//    int a[100];
    int a[100][4];

    cl_int status;

    //コンストラクタで処理
    //1,get platform id  2,get device id
    //3,create Context   4,create Command Queue
    ClUtils clUtils;

    //preloadProgramで処理、カーネルファイル読み込み
    //6,create program object 7,build program
    clUtils.preloadProgram(argv[1]);

    cl_command_queue queue = clUtils.getCmdQueue();

    //5, create memory object
    cl_mem memA=clUtils.createOutBufferRW(sizeof(a));

    //8, create kernel: カーネル関数の名前
    cl_kernel kernel=clUtils.createKernel(clProc);

    //9, set kernel parameters
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);

    size_t globalSize[] = {100};
    size_t localSize[]  = {20};

    //10, request execute kernel: input command queue
    status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, localSize, 0, NULL, NULL);

    //11, obtain results
    clUtils.enqueueReadBuffer(memA, sizeof(a), &a);

    // list results
    for(int i=0; i<sizeof(a)/sizeof(a[0]); i++)
//       printf("a[%2d]=%3d\n", i, a[i]);
       printf("global id = %2d, group id = %2d, local id = %2d\n", a[i][0], a[i][2], a[i][3]);

    //12, release resources
    clReleaseMemObject(memA);

    return 0;
}
