#ifndef __CLUTILS_H__
#define __CLUTILS_H__

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

//-------------------------------------------------------------------
// OpenCL support class
class ClUtils
{
public:
    ClUtils();
    virtual ~ClUtils();

    void             preloadProgram(const char* filename);
    cl_context       getContext() const     {return mContext;}
    cl_device_id     getDeviceId() const    {return mDeviceId;}
    cl_command_queue getCmdQueue() const    {return mCommandQueue;}
    cl_program       getProg() const        {return mProgram;}
    cl_command_queue getOutOfOrderQueue();
    cl_kernel        createKernel(const char *proc);
    cl_kernel        createAddtionalKernel(const char *proc);
    cl_mem           createInBuffer(const size_t memSize, void *pMem);
    cl_mem           createOutBuffer(const size_t memSize);
    cl_mem           createOutBuffer(const size_t memSize, unsigned char fill);
    void             enqueueReadBuffer(cl_mem mem,const size_t memSize, void *pMem);

    static void      printError(const cl_int err);

private:
    void             printBuildLog(const cl_program program,
                                const cl_device_id device) const;
    char*            loadProgramSource(const char *filename);

private:
    cl_context       mContext;
    cl_device_id     mDeviceId;
    cl_command_queue mCommandQueue;
    cl_program       mProgram;
    cl_kernel        mKernel;
};

//-------------------------------------------------------------------
// for error code
typedef struct
{
    const int   code;
    const char* msg;
} msgTbl_t;


#endif /* __CLUTILS_H__ */
