__kernel void
clCode(__global const int* a,
       __global int* minData,
       __global int* maxData)
{
    size_t i=get_global_id(0);

    *minData=min((int)*minData, a[i]);
    *maxData=min((int)*maxData, a[i]);
}
