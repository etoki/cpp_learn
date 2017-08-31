//===================================================================
// 二つの配列を乗算し、別の配列に格納する
//
// OpenCLソースを外部に出したもの。
//
//
// $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp -lOpenCL
// $ ./a.out Release/mul.cl
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

#define MAX_CL_SOURCE_SIZE  10000

//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    cl_int status;
    cl_device_id deviceID;

    float A[100], B[100], C[100];
    FILE *fp;
    char *src;
    size_t sizeOfSrc;

    // read *.cl source
    if(argc<2)
    {
        fprintf(stderr, "引数に *.cl ファイル名を指定してください.\n");
        return -1;
    }
    if((fp=fopen(argv[1], "rb"))==0)
    {
        fprintf(stderr, " %s open failed.\n", argv[1]);
        return -1;
    }
    src = new char[MAX_CL_SOURCE_SIZE];
    sizeOfSrc=fread(src, 1, MAX_CL_SOURCE_SIZE-1, fp);
    src[sizeOfSrc]='\0';
    fclose(fp);

    // initialize array
    for(int i=0; i<100;i++)
    {
        A[i]=(float)(i+1000);
        B[i]=(float)i/10.f;
    }

    // get platform id
    cl_platform_id platformId;
    status=clGetPlatformIDs(1, &platformId, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clGetPlatformIDs function failed.\n");
        return -1;
    }

    // get device id
    status=clGetDeviceIDs(platformId,
                    CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clGetDeviceIDs function failed.\n");
        return -1;
    }

    // create Context
    cl_context context=clCreateContext( NULL, 1, &deviceID,
                                            NULL, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateContext function failed.\n");
        return -1;
    }

    // create Command Queue
    cl_command_queue queue=clCreateCommandQueue(context,
                                            deviceID, 0, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateCommandQueue function failed.\n");
        return -1;
    }

    // create program object
    cl_program prog=clCreateProgramWithSource(context,
                                1, (const char**)&src, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateProgramWithSource function failed.\n");
        return -1;
    }

    // build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clBuildProgram function failed.\n");
        return -1;
    }

    delete [] src;

    // create kernel
    cl_kernel kernel=clCreateKernel(prog, "mul", &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateKernel function failed.\n");
        return -1;
    }

    // create memory object
    cl_mem memA=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(A), A, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer function for memA failed.\n");
        return -1;
    }
    cl_mem memB=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                            sizeof(B), B, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer function for memB failed.\n");
        return -1;
    }
    cl_mem memC=clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                            sizeof(C), NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer function for memC failed.\n");
        return -1;
    }

    // set kernel parameters
    status=clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clSetKernelArg function for memA failed.\n");
        return -1;
    }
    status=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clSetKernelArg function for memB failed.\n");
        return -1;
    }
    status=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clSetKernelArg function for memC failed.\n");
        return -1;
    }

    // request execute kernel
    size_t globalSize[]={sizeof(C)/sizeof(C[0])};
    status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueNDRangeKernel function failed.\n");
        return -1;
    }

    // get results
    status=clEnqueueReadBuffer(queue, memC, CL_TRUE, 0,
                                sizeof(C), C, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueReadBuffer function failed.\n");
        return -1;
    }

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
