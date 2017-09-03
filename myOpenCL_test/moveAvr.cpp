// g++ moveAvr.cpp ClUtils.cpp -lOpenCL
// ./a.out 1 moveAvr.cl
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Cexp.h"
#include "ClUtils.h"

#define MAX_SRC_SIZE (0x100000)
static const int CL_SOURCE_SIZE=10000;
static const char *clProc = "lowPass"; //カーネル関数の名前

static const float wav[]={
//#include "data.txt"
#include "task.txt"
};

//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr, "移動平均のウィンドウサイズを指定してください.\n");
        return -1;
    }

    cl_int status;

    //コンストラクタで処理
    //1,get platform id  2,get device id
    //3,create Context   4,create Command Queue
    ClUtils clUtils;

//    size_t numOfTotal=sizeof(wav)/sizeof(wav[0]);
    size_t numOfTotal=sizeof(wav)/sizeof(wav[0])/2;//タスク並列の場合
//    size_t sizeOfBuff =sizeof(wav[0])*numOfTotal;
    size_t sizeOfBuff =sizeof(wav[0])*numOfTotal*2;//タスク並列の場合
    size_t windowSize=atoi(argv[1]);
//    float *result=new float[numOfTotal];
    float *result=new float[numOfTotal*2];//タスク並列の場合

    //preloadProgramで処理、カーネルファイル読み込み
    //6,create program object 7,build program
    clUtils.preloadProgram(argv[2]);

    cl_command_queue queue = clUtils.getCmdQueue();

    //5, create memory object
    cl_mem in =clUtils.createInBuffer(sizeOfBuff, (void*)wav);
    cl_mem out=clUtils.createOutBuffer(sizeOfBuff);

    //8, create kernel: カーネル関数の名前
    cl_kernel kernel=clUtils.createKernel(clProc);

    //9, set kernel parameters
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
    status+= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&out);
    status+= clSetKernelArg(kernel, 2, sizeof(cl_uint),(void *)&numOfTotal);
    status+= clSetKernelArg(kernel, 3, sizeof(cl_uint),(void *)&windowSize);//引数の数字注意

    //10, request execute kernel: input command queue
    status=clEnqueueTask(queue, kernel, 0, NULL, NULL);

/*       //moveAvr2 ver: データ並列
    for(unsigned int i=0; i<windowSize-1; i++){
       result[i] = 0.0f;
    }
    size_t globalSize[]={numOfTotal-windowSize+1};
    status=clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalSize, 0, 0, NULL, NULL);
*/
    //11, obtain results
    clUtils.enqueueReadBuffer(out, sizeOfBuff, result);

    // list results
       //moveAvr2 ver: データ並列
//    for(size_t i=0; i<windowSize-1; i++){
//       result[i] = 0.0f;
//    }

    for(size_t i=0; i<numOfTotal; i++){
//       printf("%6d\n", (int)result[i]);
       printf("%6d,\t%6d,\n", (int)result[i*2], (int)result[i*2]+1);//タスク並列の場合
    }

    //12, release resources
    clReleaseMemObject(out);
    clReleaseMemObject(in);

    delete [] result;

    return 0;
}
