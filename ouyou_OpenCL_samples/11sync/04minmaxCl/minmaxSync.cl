__kernel void
clCode(__global const int* a,
       __global int* minData,
       __global int* maxData)
{
    size_t i=get_global_id(0);

    atomic_min(minData, a[i]);
    atomic_max(maxData, a[i]);
}
