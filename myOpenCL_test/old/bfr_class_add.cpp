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
    ClUtils clUtils;

    float A=1.1f, B=2.2f, C;

    clUtils.preloadProgram(argv[1]);
    cl_command_queue queue = clUtils.getCmdQueue();

/*
//clUtils.preloadProgramで行われる
    //kernel file read
    FILE *fp;
    char *src;
    size_t sizeOfSrc;

    if((fp = fopen(argv[1], "rb"))==0)
    {
        fprintf(stderr, " %s open failed.\n", argv[1]);
        return -1;
    }
    src = new char[CL_SOURCE_SIZE];
    sizeOfSrc = fread(src, 1, CL_SOURCE_SIZE-1, fp);
    src[sizeOfSrc] = '\0';
    fclose(fp);
*/
/*
    // カーネルファイル直書き
    static const char *src[]=
    {
        "__kernel void\n\
         add(__global const float *A,\n\
             __global const float *B,\n\
             __global float *C)\n\
         {\n\
             *C=*A+*B;\n\
         }\n"
    };
*/
/*
//constructorで行われる
    //1, get platform id
    cl_platform_id platformId;
    status=clGetPlatformIDs(1, &platformId, NULL);

    //2, get device id
    cl_device_id deviceID;
    status=clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);

    //3, create Context
    cl_context context=clCreateContext( NULL, 1, &deviceID, NULL, NULL, NULL);

    //4, create Command Queue
    cl_command_queue queue=clCreateCommandQueue(context, deviceID, 0, NULL);
*/

    cl_mem memA=clUtils.createInBuffer(sizeof(A), &A);
    cl_mem memB=clUtils.createInBuffer(sizeof(B), &B);
    cl_mem memC=clUtils.createOutBuffer(sizeof(C));

/*
//createInBufferとcreateOutBufferで行われる
    //5, create memory object
    cl_mem memA=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(A), &A, NULL);
    cl_mem memB=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(B), &B, NULL);
    cl_mem memC=clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(C), NULL, NULL);
*/

/*
//preloadProgramで行われる
    //6, create program object
    cl_program prog=clCreateProgramWithSource(context, 1, (const char**)&src, NULL, NULL);

    //7, build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);
*/

    //8, create kernel: カーネル関数の名前
//    cl_kernel kernel=clCreateKernel(prog, "add", NULL);
    cl_kernel kernel=clUtils.createKernel(clProc);

    //9, set kernel parameters
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    status+= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    status+= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);

    //10, request execute kernel: input command queue
    status=clEnqueueTask(queue, kernel, 0, NULL, NULL);

    //11, obtain results
//    status=clEnqueueReadBuffer(queue, memC, CL_TRUE, 0, sizeof(C), &C, 0, NULL, NULL);
    clUtils.enqueueReadBuffer(memC, sizeof(C), &C);

    // list results
    printf("%f + %f = %f\n", A, B, C);

    //12, release resources
    clReleaseMemObject(memC);
    clReleaseMemObject(memB);
    clReleaseMemObject(memA);

/*
//デストラクタで処理
    clReleaseKernel(kernel);
    clReleaseProgram(prog);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
*/

    return 0;
}
