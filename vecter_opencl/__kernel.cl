__kernel void
vecterTest(__global const char2 *a, __global const char2 *b, __global const char2 *c)
{
   int i = get_global_id(0);

   c[i] = a[i] + b[i];
}
