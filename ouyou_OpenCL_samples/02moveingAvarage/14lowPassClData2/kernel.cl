__kernel void
lowPass(__global const float2 *in,
        __global       float2 *out,
        const uint windowSize)
{
    size_t x=get_global_id(0);

    float2 value=(float2)0.0f;
    for(unsigned int i=x; i<x+windowSize; i++)
        value+=in[i];

    out[x+windowSize-1]=value/(float2)windowSize;
}
