#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include <stdio.h>


static const size_t dataSize=100;
static const size_t MAX_CL_SOURCE_SIZE=10000;
static const char* kernelName="vecterTest";


//-------------------------------------------------------------------
// read kernel source
char*
readKernelSource(int argc, char* argv[])
{
	FILE *fp;
	char *src;
	size_t sizeOfSrc;

	if(argc<2){
		fprintf(stderr, "引数にファイル名を指定してしてください\n");
		return NULL;
	}
	if((fp=fopen(argv[1], "rb"))==0)
	{
		fprintf(stderr, "%s open failed\n", argv[1]);
		return NULL;
	}
	src = new char[MAX_CL_SOURCE_SIZE];
	sizeOfSrc = fread(src, 1, MAX_CL_SOURCE_SIZE-1, fp);
	src[sizeOfSrc]='\0';

	fclose(fp);

	return src;
}


//-------------------------------------------------------------------
// initialize data

void
initData(cl_char2 a[], cl_char2 b[])
{
	for(int i=0; i<dataSize; i++)
	{
		for(int dim=0; dim<2; dim++)
		{
			a[i].s[dim] = (cl_char)(i+1+dim);
			b[i].s[dim] = (cl_char)(i*2+dim);
		}
	}
}


//-------------------------------------------------------------------
// print data

void
printData(cl_char2 a[], cl_char2 b[], cl_char2 c[])
{
	printf("(a + b = c)\n");

	for(int i=0; i<10; i++)
	{
		for(int dim=0; dim<2; dim++)
		{
			printf("dim%2d: %3d + %3d = %3d\n", dim, (unsigned char)a[i].s[dim],(unsigned char)b[i].s[dim],(unsigned char)c[i].s[dim] );
		}
		printf("\n");
	}
}

//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    //common~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    cl_int status;
    cl_platform_id platformId;
    cl_device_id deviceID;

    cl_char2 a[dataSize], b[dataSize], c[dataSize];
    initData(a,b);

    char *src=readKernelSource(argc, argv);
    if(src==NULL)
       return -1;

    // get platform id
    status=clGetPlatformIDs(1, &platformId, NULL);
    if(status!=CL_SUCCESS)
        return -1;

    // get device id
    status=clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, NULL);
    if(status!=CL_SUCCESS)
        return -1;
                                                    
    // create Context
    cl_context context=clCreateContext( NULL, 1, &deviceID, NULL, NULL, &status);
    if(status!=CL_SUCCESS)
        return -1;

    // create Command Queue
    cl_command_queue queue=clCreateCommandQueue(context, deviceID, 0, &status);
    if(status!=CL_SUCCESS)
        return -1;

    // create program object
    cl_program prog=clCreateProgramWithSource(context, 1, (const char**)&src, NULL, &status);
    if(status!=CL_SUCCESS)
        return -1;
                                                   
    // build program
    status=clBuildProgram(prog, 1, &deviceID, NULL, NULL, NULL);
    if(status!=CL_SUCCESS)
        return -1;

    // create kernel
    cl_kernel kernel=clCreateKernel(prog, kernelName, &status);
    if(status!=CL_SUCCESS)
        return -1;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~common

    cl_mem memA = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(a), a, &status);
    if(status!=CL_SUCCESS)
        return -1;

    cl_mem memB = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(b), b, &status);
    if(status!=CL_SUCCESS)
        return -1;

    cl_mem memC = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(c), c, &status);
    if(status!=CL_SUCCESS)
        return -1;

    // set kernel parameters
    status =clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memA);
    status+=clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memB);
    status+=clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memC);
    if(status!=CL_SUCCESS)
        return -1;

    // request execute kernel
    size_t globalSize=sizeof(c)/sizeof(c[0]);
    status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, 0, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        return -1;

    // obtain results
    status=clEnqueueReadBuffer(queue, memC, CL_TRUE, 0, sizeof(c), c, 0, NULL, NULL);
    if(status!=CL_SUCCESS)
        return -1;

    printData(a,b,c);
    delete [] src;

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
