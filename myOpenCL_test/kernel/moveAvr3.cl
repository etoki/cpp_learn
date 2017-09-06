__kernel void
lowPass(__global const float2 *in,
        __global       float2 *out,
        const uint numOfTotal,
        const uint windowSize)
{
    for(size_t i=0; i<windowSize-1; i++)
        out[i]=(float2)0.0f;

    float2 value=(float2)0.0f;
    for(size_t i=0; i<windowSize; i++)
        value+=in[i];

    out[windowSize-1]=value/(float2)windowSize;
    for(size_t i=windowSize; i<numOfTotal; i++)
    {
        value+=(in[i]-in[i-windowSize]);
        out[i]=value/(float2)windowSize;
    }
}
