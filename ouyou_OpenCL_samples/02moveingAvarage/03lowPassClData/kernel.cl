__kernel void
lowPass(__global const float *in,
        __global float *out,
        const uint windowSize)
{
    unsigned int x=get_global_id(0);

    float value=0.0f;
    for(unsigned int i=x; i<x+windowSize; i++)
        value+=in[i];

    out[x+windowSize-1]=value/windowSize;
}
