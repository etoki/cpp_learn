//===================================================================
// low pass, moving average, CL
//
// $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp -lOpenCL
// $ cp Release/kernel.cl .
// $ ./a.out 1000 | more
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
#include <stdlib.h>


static const int CL_SOURCE_SIZE=10000;
static const char* clName="kernel.cl";
static const char* clProc="lowPass";

static const float wav[]=
{
#include "../dim2.txt"
};


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    cl_int status;
    cl_platform_id platformId;
    cl_device_id deviceID;

    if(argc<2)
    {
        fprintf(stderr, "移動平均のウィンドウサイズを指定してください.\n");
        return -1;
    }

    FILE *fp;
    char *src;
    size_t sizeOfSrc;

    if((fp=fopen(clName, "rb"))==0)
    {
        fprintf(stderr, " %s open failed.\n", clName);
        return -1;
    }
    src=new char[CL_SOURCE_SIZE];
    sizeOfSrc=fread(src, 1, CL_SOURCE_SIZE-1, fp);
    src[sizeOfSrc]='\0';
    fclose(fp);

    size_t numOfTotal=sizeof(wav)/sizeof(wav[0])/2; // <--- /2
    size_t sizeOfBuff=sizeof(wav[0])*numOfTotal*2;
    size_t windowSize=atoi(argv[1]);

    float *result=new float[numOfTotal*2];          // <--- *2

    status=clGetPlatformIDs(1, &platformId, NULL);  // get platform id
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clGetPlatformIDs failed.\n");
        return -1;
    }

    status=clGetDeviceIDs(platformId,               // get device id
                    CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clGetDeviceIDs failed.\n");
        return -1;
    }

                                                    // create Context
    cl_context context=clCreateContext( NULL, 1, &deviceID,
                                            NULL, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateContext failed.\n");
        return -1;
    }

                                                    // create Command Queue
    cl_command_queue queue=clCreateCommandQueue(context,
                                            deviceID, 0, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreaetCommandQueue failed.\n");
        return -1;
    }

                                                    // create program object
    cl_program prog=clCreateProgramWithSource(context,
                                1, (const char**)&src, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateProgramWithSource failed.\n");
        return -1;
    }

                                                    // build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clBuildProgram failed.\n");
        return -1;
    }

                                                    // create kernel
    cl_kernel kernel=clCreateKernel(prog, clProc, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateKernel failed.\n");
        return -1;
    }

                                                    // create memory object
    cl_mem in=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
                                            sizeOfBuff, (void*)wav, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer failed.\n");
        return -1;
    }

    cl_mem out=clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                            sizeOfBuff, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer failed.\n");
        return -1;
    }

                                                    // set kernel parameters
    status =clSetKernelArg(kernel, 0, sizeof(cl_mem),  (void *)&in);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_mem),  (void *)&out);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_uint), (void *)&numOfTotal);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_uint), (void *)&windowSize);
    if(status!=0)
    {
        fprintf(stderr, "clSetKernelArg for orgImg failed.\n");
        return -1;
    }

                                                    // request execute kernel
    status=clEnqueueTask(queue, kernel, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueTask failed.");
        return -1;
    }

                                                    // obtain results
    status=clEnqueueReadBuffer(queue, out, CL_TRUE, 0,
                                sizeOfBuff, result, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueReadBuffer failed.\n");
        return -1;
    }

    for(size_t i=0; i<numOfTotal; i++)
        printf("%6d,\t%6d,\n", (int)result[i*2], (int)result[(i*2)+1]);

                                                    // release resources
    clReleaseMemObject(out);
    clReleaseMemObject(in);
    clReleaseKernel(kernel);
    clReleaseProgram(prog);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    delete [] result;
    delete [] src;

    return 0;
}
