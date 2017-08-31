// min, max task parallel

__kernel void
minCl(__global const int* a,
      __global int* minData)
{
    size_t i=get_global_id(0);

    *minData=min((int)minData, a[i]);
}

__kernel void
maxCl(__global const int* a,
      __global int* maxData)
{
    size_t i=get_global_id(0);

    *maxData=min((int)maxData, a[i]);
}
