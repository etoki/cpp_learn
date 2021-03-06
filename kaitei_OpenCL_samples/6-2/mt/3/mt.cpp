#include <stdlib.h>
#include <CL/cl.h>
#include <stdio.h>
#include <math.h>

typedef struct mt_struct_s {
    cl_uint aaa;
    cl_int mm,nn,rr,ww;
    cl_uint wmask,umask,lmask;
    cl_int shift0, shift1, shiftB, shiftC;
    cl_uint maskB, maskC;
} mt_struct ;

#include "mts.h"

#define MAX_SOURCE_SIZE (0x100000)

int main(int argc, char **argv)
{
    cl_int num_rand = 4096*256*32;        /* 生成する乱数の数 */
    int count_all, i;
    cl_uint num_generator;
    unsigned int num_work_item;
    double pi;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_platforms;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;
    cl_program program = NULL;
    cl_kernel kernel_mt = NULL, kernel_pi = NULL;
    size_t kernel_code_size;
    char *kernel_src_str;
    cl_uint *result;
    cl_int ret;
    FILE *fp;
    cl_mem rand, count;
    size_t global_item_size[3], local_item_size[3];
    cl_mem dev_mts;
    cl_event ev_mt_end, ev_pi_end, ev_copy_end;
    cl_ulong prof_start, prof_mt_end, prof_pi_end, prof_copy_end;
    cl_uint num_compute_unit;
    int mts_size;
    mt_struct *mts = NULL;

    if (argc >= 2) {
        int n = atoi(argv[1]);
        if (n == 128) {
            mts = mts128;
            mts_size = sizeof(mts128);
            num_generator = 128;
            num_rand /= 128;
        }
        if (n == 256) {
            mts = mts256;
            mts_size = sizeof(mts256);
            num_generator = 256;
            num_rand /= 256;
        }
    }

    if (mts == NULL) {
        mts = mts512;
        mts_size = sizeof(mts512);
        num_generator = 512;
        num_rand /= 512;
    } 

    clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, 
                   &ret_num_devices);
    context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
    result = (cl_uint*)malloc(sizeof(cl_uint)*num_generator);

    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

    fp = fopen("mt.cl", "r");
    kernel_src_str = (char*)malloc(MAX_SOURCE_SIZE);
    kernel_code_size = fread(kernel_src_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    /* 出力先バッファの作成 */
    rand = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint)*num_rand*num_generator, NULL, &ret);
    count = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint)*num_generator, NULL, &ret);

    /* プログラムのビルド */
    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src_str,
                                        (const size_t *)&kernel_code_size, &ret);
    clBuildProgram(program, 1, &device_id, "", NULL, NULL);
    kernel_mt = clCreateKernel(program, "genrand", &ret);
    kernel_pi = clCreateKernel(program, "calc_pi", &ret);

    /* 入力パラメータの作成 */
    dev_mts = clCreateBuffer(context, CL_MEM_READ_WRITE, mts_size, NULL, &ret);
    clEnqueueWriteBuffer(command_queue, dev_mts, CL_TRUE, 0, mts_size, mts, 0, NULL, NULL);

    /* カーネル引数の設定 */
    clSetKernelArg(kernel_mt, 0, sizeof(cl_mem), (void*)&rand); /* 乱数値(genrandの出力)*/
    clSetKernelArg(kernel_mt, 1, sizeof(cl_mem), (void*)&dev_mts); /* 生成パラメータ(genrandの入力) */
    clSetKernelArg(kernel_mt, 2, sizeof(num_rand), &num_rand); /* 生成する乱数の個数 */
    clSetKernelArg(kernel_mt, 3, sizeof(num_generator), &num_generator); /* 生成パラメータの数 */

    clSetKernelArg(kernel_pi, 0, sizeof(cl_mem), (void*)&count); /* 円に含まれるカウント(calc_piの出力) */
    clSetKernelArg(kernel_pi, 1, sizeof(cl_mem), (void*)&rand);  /* 乱数列(calc_piの入力) */
    clSetKernelArg(kernel_pi, 2, sizeof(num_rand), &num_rand);   /* 乱数の個数 */
    clSetKernelArg(kernel_pi, 3, sizeof(num_generator), &num_generator); /* 生成パラメータの数 */

    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num_compute_unit, NULL);

    if (num_compute_unit > num_generator)
        num_compute_unit = num_generator;

    /* ワークアイテムの数 (パラメータの数を、並列数で割った値を繰り上げた値) */
    num_work_item = (num_generator+(num_compute_unit-1)) / num_compute_unit;

    /* ワークグループの数、ワークアイテムの数を設定 */
    global_item_size[0] = num_work_item * num_compute_unit; global_item_size[1] = 1; global_item_size[2] = 1;
    local_item_size[0] = num_work_item;  local_item_size[1] = 1;  local_item_size[2] = 1;

    /* 乱数列を生成 */
    clEnqueueNDRangeKernel(command_queue, kernel_mt, 1, NULL, global_item_size, local_item_size, 0, NULL, &ev_mt_end);

    /* pi を計算 */
    clEnqueueNDRangeKernel(command_queue, kernel_pi, 1, NULL, global_item_size, local_item_size, 0, NULL, &ev_pi_end);

    /* 結果の取得 */
    clEnqueueReadBuffer(command_queue, count, CL_TRUE, 0, sizeof(cl_uint)*num_generator, result, 0, NULL, &ev_copy_end);

    /* piの計算 */
    count_all = 0;
    for (i=0; i<(int)num_generator; i++) {
        count_all += result[i];
    }

    pi = ((double)count_all)/(num_rand * num_generator) * 4;
    printf("pi = %f\n", pi);

    /* 処理時間の取得 */
    clGetEventProfilingInfo(ev_mt_end, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &prof_start, NULL);
    clGetEventProfilingInfo(ev_mt_end, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &prof_mt_end, NULL);
    clGetEventProfilingInfo(ev_pi_end, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &prof_pi_end, NULL);
    clGetEventProfilingInfo(ev_copy_end, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &prof_copy_end, NULL);

    printf("    mt: %f[ms]\n"
           "    pi: %f[ms]\n"
           "    copy: %f[ms]\n",
           (prof_mt_end - prof_start)/(1000000.0),
           (prof_pi_end - prof_mt_end)/(1000000.0),
           (prof_copy_end - prof_pi_end)/(1000000.0));

    clReleaseEvent(ev_mt_end);
    clReleaseEvent(ev_pi_end);
    clReleaseEvent(ev_copy_end);

    clReleaseMemObject(rand);
    clReleaseMemObject(count);
    clReleaseKernel(kernel_mt);
    clReleaseKernel(kernel_pi);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    free(kernel_src_str);
    free(result);
    return 0;
}
