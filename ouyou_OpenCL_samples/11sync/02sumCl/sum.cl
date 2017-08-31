__kernel void
clCode(__global const int* in,
       __global int* sum)
{
    size_t i=get_global_id(0);

    atomic_add(sum, in[i]);
}
