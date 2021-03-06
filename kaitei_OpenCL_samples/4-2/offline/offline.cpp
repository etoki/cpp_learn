#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_BINARY_SIZE (0x100000)

int main()
{
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_mem memobj = NULL;
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;

    float mem[MEM_SIZE];

    FILE *fp;
    char fileName[] = "./kernel.clbin";
    size_t binary_size;
    char *binary_buf;
    cl_int binary_status;
    cl_int i;

    /* カーネルを含むオブジェクトファイルをロード */
    fp = fopen(fileName, "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    binary_buf = (char *)malloc(MAX_BINARY_SIZE);
    binary_size = fread( binary_buf, 1, MAX_BINARY_SIZE, fp );
    fclose( fp );

    /* データを初期化 */
    for( i = 0; i < MEM_SIZE; i++ ) {
        mem[i] = i;
    }

    /* プラットフォーム・デバイスの情報の取得 */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    /* OpenCLコンテキストの作成 */
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
  
    /* コマンドキューの作成 */
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  
    /* メモリバッファの作成 */
    memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(float), NULL, &ret);

    /* メモリバッファにデータを転送 */
    ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem, 0, NULL, NULL);

    /* 読み込んだバイナリからカーネルプログラムを作成 */
    program = clCreateProgramWithBinary(context, 1, &device_id, (const size_t *)&binary_size, 
                                        (const unsigned char **)&binary_buf, &binary_status, &ret);
    
    /* OpenCLカーネルの作成 */
    kernel = clCreateKernel(program, "vecAdd", &ret);
    printf("err:%d\n", ret);

    /* OpenCLカーネル引数の設定 */
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);

    size_t global_work_size[3] = {MEM_SIZE, 0, 0};
    size_t local_work_size[3]  = {MEM_SIZE, 0, 0};

    /* OpenCLカーネルを実行 */
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);

    /* メモリバッファから結果を取得 */
    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem, 0, NULL, NULL);

    /* 結果の表示 */
    for(i=0; i<MEM_SIZE; i++) {
        printf("mem[%d] : %f\n", i, mem[i]);
    }
  
    /* 終了処理 */
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(binary_buf);

    return 0;
}

