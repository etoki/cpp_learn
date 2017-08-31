//===================================================================
// 二つの配列を乗算し、別の配列に格納する
//
// プログラムを見やすくするため、一切のエラーチェックを省略。
// 何かエラーがあったときは、原因究明に時間がかかる。
//
//
// $ gcc -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp -lOpenCL
// $ ./a.out
// (A * B = C)
// 1000.000000 * 0.000000 = 0.000000
// 1001.000000 * 0.100000 = 100.099998
// 1002.000000 * 0.200000 = 200.400009
// 1003.000000 * 0.300000 = 300.900024
// 1004.000000 * 0.400000 = 401.600006
// 1005.000000 * 0.500000 = 502.500000
// 1006.000000 * 0.600000 = 603.600037
// 1007.000000 * 0.700000 = 704.899963
// 1008.000000 * 0.800000 = 806.400024
// 1009.000000 * 0.900000 = 908.099976
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
    cl_device_id deviceID;

    float A[100], B[100], C[100];

    // initialize array
    for(int i=0; i<100;i++)
    {
        A[i]=(float)(i+1000);
        B[i]=(float)i/10.f;
    }

    // get platform id
    cl_platform_id platformId;
    status=clGetPlatformIDs(1, &platformId, NULL);

    // get device id
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
        mul(__global const float *A,\n\
            __global const float *B,\n\
            __global float *C)\n\
        {\n\
            int i=get_global_id(0);\n\
            C[i]=A[i]*B[i];\n\
        }\n"
    };
    cl_program prog=clCreateProgramWithSource(context,
                                1, (const char**)&src, NULL, NULL);

    // build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);

    // create kernel
    cl_kernel kernel=clCreateKernel(prog, "mul", NULL);

    // create memory object
    cl_mem memA=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(A), A, NULL);
    cl_mem memB=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(B), B, NULL);
    cl_mem memC=clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(C), NULL, NULL);

    // set kernel parameters
    status=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    status=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    status=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);

    // request execute kernel
    size_t globalSize[]={sizeof(C)/sizeof(C[0])};
    status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                    globalSize, 0, 0, NULL, NULL);

    // get results
    status=clEnqueueReadBuffer(queue, memC, CL_TRUE, 0,
                                sizeof(C), C, 0, NULL, NULL);

    // list results
    printf("(A * B = C)\n");
    for(int i = 0; i < 10; i++)
        printf("%f * %f = %f\n", A[i], B[i], C[i]);

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
