// min, max task parallel

__kernel void
minCl(__global const int* a,
      __global int* minData)
{
    size_t i=get_global_id(0);

    atomic_min(minData, a[i]);
}

__kernel void
maxCl(__global const int* a,
      __global int* maxData)
{
    size_t i=get_global_id(0);

    atomic_max(maxData, a[i]);
}
