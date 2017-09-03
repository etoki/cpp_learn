#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include "Cexp.h"
#include "ClUtils.h"


//-------------------------------------------------------------------
// constructor
//
ClUtils::ClUtils()
    : mContext((cl_context)0), mDeviceId((cl_device_id)0),
      mCommandQueue((cl_command_queue)0), mProgram((cl_program)0),
      mKernel((cl_kernel)0)
{
    cl_int status;
    cl_platform_id platformId;
    cl_uint num_platforms;

                                                    // get platforms
    status=clGetPlatformIDs(1, &platformId, &num_platforms);
    if(status!=CL_SUCCESS || num_platforms<=0)
        throw Cexp("clGetPlatformIDs failed.");

    status=clGetDeviceIDs(platformId,               // get device id
                    CL_DEVICE_TYPE_DEFAULT,
                    //CL_DEVICE_TYPE_CPU,
                    //CL_DEVICE_TYPE_GPU,
                    1, &mDeviceId, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clGetDeviceIDs failed.");

    mContext=clCreateContext(NULL, 1, &mDeviceId,   // create Context
                                NULL, NULL, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateContext failed.");

    mCommandQueue=clCreateCommandQueue(mContext,    // create Command Queue
                            mDeviceId, 0, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateCommandQueue failed.");
}

//-------------------------------------------------------------------
// デストラクタ
//
ClUtils::~ClUtils()
{
    if(mContext!=(cl_context)0)
        clReleaseContext(mContext);

    if(mCommandQueue!=(cl_command_queue)0)
        clReleaseCommandQueue(mCommandQueue);

    if(mProgram!=(cl_program)0)
        clReleaseProgram(mProgram);

    if(mKernel!=(cl_kernel)0)
        clReleaseKernel(mKernel);
}

//-------------------------------------------------------------------
// read kernel source form file
//
void
ClUtils::preloadProgram(const char* filename)
{
    cl_int status;
    char *source=0;

    try
    {
        source=loadProgramSource(filename);     // read kernel program
    }
    catch(Cexp exp)
    {
        throw Cexp("failed to load kernel file.");
    }

                                                // create kernel with source
    mProgram=clCreateProgramWithSource(mContext,
                1, (const char **)&source, NULL, &status);
    if(mProgram==(cl_program)0)
    {
        delete [] source;
        throw Cexp("clCreateProgramWithSource failed.");
    }

                                                // build program
    status=clBuildProgram(mProgram, 1,
                        &mDeviceId, NULL, NULL, NULL);
    if(status!=CL_SUCCESS)
    {
        printBuildLog(mProgram, mDeviceId);
        delete [] source;
        throw Cexp("clBuildProgram failed.");
    }

    delete [] source;
}


//-------------------------------------------------------------------
// print build log
//
// program: program object
// device:  device id
//
void
ClUtils::printBuildLog(const cl_program program,
                       const cl_device_id device) const
{
    cl_int status;
    size_t size_ret;

    char buffer[4096];
    status=clGetProgramBuildInfo(program, device,
                CL_PROGRAM_BUILD_LOG, sizeof(buffer)-1,
                                        buffer, &size_ret);
    if(status!=CL_SUCCESS)
    {
        printf("clGetProgramInfo failed.\n");
        printError(status);
    }
    else
    {
        buffer[size_ret]='\0';
        puts("--------------- build log --------------");
        printf("%s\n", buffer);
        puts("----------- end of build log -----------");
    }
}

//-------------------------------------------------------------------
// load program source
//
// must release buffer by caller
//
char*
ClUtils::loadProgramSource(const char *filename)
{
    FILE *fp;
    struct stat statbuf;
    char *source;

    fp=fopen(filename, "rb");
    if(fp==0)
        throw Cexp("kernel program file open failed.");

    if(stat(filename, &statbuf)!=0)
        throw Cexp("stat failed.");

    source=new char[statbuf.st_size+1];
    fread(source, statbuf.st_size, 1, fp);
    source[statbuf.st_size]='\0';

    fclose(fp);

    return source;
}


//-------------------------------------------------------------------
// get out of order queue
cl_command_queue
ClUtils::getOutOfOrderQueue()
{
    cl_int status;

    if(mCommandQueue!=(cl_command_queue)0)
        clReleaseCommandQueue(mCommandQueue);

                                                    // create Command Queue
    mCommandQueue=clCreateCommandQueue(mContext, mDeviceId,
            CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateCommandQueue failed.");

    return mCommandQueue;
}


//-------------------------------------------------------------------
// create kernel
cl_kernel
ClUtils::createKernel(const char *proc)
{
    mKernel=clCreateKernel(mProgram, proc, NULL);
    if(mKernel==(cl_kernel)0)
        throw Cexp("clCreateKernel failed.");

    return mKernel;
}


//-------------------------------------------------------------------
// create kernel
cl_kernel
ClUtils::createAddtionalKernel(const char *proc)
{
    cl_kernel kernel=clCreateKernel(mProgram, proc, NULL);
    if(kernel==(cl_kernel)0)
        throw Cexp("clCreateKernel failed.");

    return kernel;
}

//-------------------------------------------------------------------
// create memory object(in)
cl_mem
ClUtils::createInBuffer(const size_t memSize, void *pMem)
{
    cl_int status;

    cl_mem mem=clCreateBuffer(mContext,
                            CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR,
                                    memSize, pMem,  &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateBuffer failed.");

    return mem;
}

//-------------------------------------------------------------------
// create memory object(out)
cl_mem
ClUtils::createOutBuffer(const size_t memSize)
{
    cl_int status;

    cl_mem mem=clCreateBuffer(mContext, CL_MEM_WRITE_ONLY,
                                    memSize, NULL,  &status);
    if(status!=CL_SUCCESS)
        throw Cexp("clCreateBuffer failed.");

    return mem;
}

//-------------------------------------------------------------------
// create memory object(out) and fill it
cl_mem
ClUtils::createOutBuffer(const size_t memSize, const unsigned char fill)
{
    cl_mem mem=this->createOutBuffer(memSize);

#define opencl12                                    // OpenCL 1.1の場合コメントアウト
#ifdef opencl12                                     // opencl 1.2
    cl_int status=clEnqueueFillBuffer(mCommandQueue, mem, &fill,
                    1, 0, memSize,  0, NULL, NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueFillBuffer failed.");
#endif
    return mem;
}

//-------------------------------------------------------------------
// enqueue ReadBuffer
void
ClUtils::enqueueReadBuffer(cl_mem mem, const size_t memSize, void *pMem)
{
    cl_int status=clEnqueueReadBuffer(mCommandQueue, 
                                      mem,//
                                      CL_TRUE, 
                                      0, 
                                      memSize,//
                                      pMem,//
                                      0, 
                                      NULL, 
                                      NULL);
    if(status!=CL_SUCCESS)
        throw Cexp("clEnqueueReadBuffer failed.");
}

//-------------------------------------------------------------------
// print error message
void
ClUtils::printError(const cl_int err)
{
    msgTbl_t mTbl[]={

    {CL_BUILD_PROGRAM_FAILURE,              "Program build failed"              },
    {CL_COMPILER_NOT_AVAILABLE,             "OpenCL compiler is not available"  },
    {CL_DEVICE_NOT_AVAILABLE,               "Device is not available"           },
    {CL_DEVICE_NOT_FOUND,                   "Device not found"                  },
    {CL_IMAGE_FORMAT_NOT_SUPPORTED,         "Image format is not supported"     },
    {CL_INVALID_ARG_INDEX,                  "Invalid arg index"                 },
    {CL_INVALID_ARG_SIZE,                   "Invalid arg size"                  },
    {CL_INVALID_BINARY,                     "Invalid binary"                    },
    {CL_INVALID_BUFFER_SIZE,                "Invalid buffer size"               },
    {CL_INVALID_BUILD_OPTIONS,              "Invalid build options"             },
    {CL_INVALID_CONTEXT,                    "Invalid context"                   },
    {CL_INVALID_EVENT_WAIT_LIST,            "Invalid event wait list"           },
    {CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,    "Invalid image format descriptor"   },
    {CL_INVALID_IMAGE_SIZE,                 "Invalid image size"                },
    {CL_INVALID_KERNEL_ARGS,                "Invalid kernel args"               },
    {CL_INVALID_KERNEL_NAME,                "Invalid kernel name"               },
    {CL_INVALID_MEM_OBJECT,                 "Invalid memory object"             },
    {CL_INVALID_OPERATION,                  "Invalid operation"                 },
    {CL_INVALID_PROGRAM,                    "invalid program"                   },
    {CL_INVALID_PROGRAM_EXECUTABLE,         "invalid program executable"        },
    {CL_INVALID_SAMPLER,                    "invalid sampler"                   },
    {CL_INVALID_VALUE,                      "invalid value"                     },
    {CL_INVALID_WORK_DIMENSION,             "invalid work dimension"            },
    {CL_INVALID_WORK_GROUP_SIZE,            "Invalid work group size"           },
    {CL_INVALID_WORK_ITEM_SIZE,             "Invalid work item size"            },
    {CL_MEM_OBJECT_ALLOCATION_FAILURE,      "Memory object allocation failure"  },
    {CL_OUT_OF_HOST_MEMORY,                 "Out of host memory"                },
    {CL_OUT_OF_RESOURCES,                   "Out of resources"                  },
    {CL_SUCCESS,                            "Succeeded"                         },
    {-1,                                    NULL                                }

    };

    int i=0;
    while(mTbl[i].msg!=NULL)
    {
        if(mTbl[i].code==err)
        {
            fputs(mTbl[i].msg, stderr);
            break;
        }
        i++;
    }

    if(mTbl[i].msg==NULL)
        fprintf(stderr, "Unknown error: %d\n", err);
}
