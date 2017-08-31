__kernel void
clCode(__global const int* in,
       __global int* sum)
{
    size_t i=get_global_id(0);

    *sum+=in[i];
}
