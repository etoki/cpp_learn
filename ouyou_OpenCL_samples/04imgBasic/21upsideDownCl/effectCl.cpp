//===================================================================
// effectCl.cpp
//
// cl ..\01neg\negMain.cpp ..\01neg\bitmap.cpp effectCl.cpp
//
// $ g++ -I /opt/AMDAPP/include/ ../01imgCl/main.cpp -L /opt/AMDAPP/lib/x86_64/ effectCl.cpp ../bitmap/bitmap.cpp -lOpenCL
// $ ./a.out '/home/k/OpenCLimage/tstImg/Lenna.bmp' a.bmp
// Bitmap Info. = 256 x 256, 24 bits per pixel.
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


static const int CL_SOURCE_SIZE=10000;
static const char *clName="clCode.cl";
static const char* clProc="clCode";

/********************************************************************
//-------------------------------------------------------------------
// effect
void
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{
    for(unsigned int y=0; y<height; y++)
        for(unsigned int x=0 ; x<width*3 ; x++)
            pBitmap[y*width*3+x]=(unsigned char)(255-pBitmap[y*width*3+x]);
}
********************************************************************/


//-------------------------------------------------------------------
// effect
int
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{
    cl_int status;
    cl_platform_id platformId;
    cl_device_id deviceID;

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

    size_t datasize=width*3*height;

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
    cl_mem in=clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR,
                                sizeof(cl_uchar)*datasize, pBitmap, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer failed.\n");
        return -1;
    }

    cl_mem out=clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                sizeof(cl_uchar)*datasize, NULL, &status);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clCreateBuffer failed.\n");
        return -1;
    }

                                                    // set kernel parameters
    status =clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *)&width);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *)&height);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_mem),  (void *)&in);
    status+=clSetKernelArg(kernel, 3, sizeof(cl_mem),  (void *)&out);
    if(status!=0)
    {
        fprintf(stderr, "clSetKernelArg for orgImg failed.\n");
        return -1;
    }

                                                    // request execute kernel
    cl_uint work_dim=2;
    size_t globalSize[]={width*3, height};
    status=clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                    globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueNDRangeKernel failed.");
        return -1;
    }

                                                    // obtain results
    status=clEnqueueReadBuffer(queue, out, CL_TRUE, 0,
                        sizeof(cl_uchar)*datasize, pBitmap, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        fprintf(stderr, "clEnqueueReadBuffer failed.\n");
        return -1;
    }

                                                    // release resources
    clReleaseMemObject(out);
    clReleaseMemObject(in);
    clReleaseKernel(kernel);
    clReleaseProgram(prog);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    delete [] src;

    return 0;
}
