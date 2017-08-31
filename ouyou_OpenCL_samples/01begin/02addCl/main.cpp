//===================================================================
// 二つ変数を加算し、別の変数に格納する,C/C++で書いたもの。
//
// Linux, AMD OpenCL開発環境の例:
//  $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp -lOpenCL
//  $ ./a.out
//  1.100000 + 2.200000 = 3.300000
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include <stdio.h>


//-------------------------------------------------------------------
// main
int
main()
{
    cl_int status;
    float A=1.1f, B=2.2f, C;

    // get platform id
    cl_platform_id platformId;
    status=clGetPlatformIDs(1, &platformId, NULL);

    // get device id
    cl_device_id deviceID;
    status=clGetDeviceIDs(platformId,
                    CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);

    // create Context
    cl_context context=clCreateContext( NULL, 1, &deviceID,
                                            NULL, NULL, NULL);

    // create Command Queue
    cl_command_queue queue=clCreateCommandQueue(context,
                                            deviceID, 0, NULL);

    // create program object
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
    cl_program prog=clCreateProgramWithSource(context,
                                1, (const char**)&src, NULL, NULL);

    // build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);

    // create kernel
    cl_kernel kernel=clCreateKernel(prog, "add", NULL);

    // create memory object
    cl_mem memA=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(A), &A, NULL);
    cl_mem memB=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(B), &B, NULL);
    cl_mem memC=clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(C), NULL, NULL);

    // set kernel parameters
    status=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    status=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    status=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);

    // request execute kernel
    status=clEnqueueTask(queue, kernel, 0, NULL, NULL);

    // obtain results
    status=clEnqueueReadBuffer(queue, memC, CL_TRUE, 0,
                                sizeof(C), &C, 0, NULL, NULL);

    // list results
    printf("%f + %f = %f\n", A, B, C);

    // release resources
    clReleaseMemObject(memC);
    clReleaseMemObject(memB);
    clReleaseMemObject(memA);
    clReleaseKernel(kernel);
    clReleaseProgram(prog);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
