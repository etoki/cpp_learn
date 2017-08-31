__kernel void
lowPass(__global const float *in,
        __global float *out,
        const uint numOfTotal,
        const uint windowSize)
{
    for(size_t i=0; i<windowSize-1; i++)
        out[i]=0.0f;

    float value=0.0f;
    for(size_t i=0; i<windowSize; i++)
        value+=in[i];

    out[windowSize-1]=value/windowSize;
    for(size_t i=windowSize; i<numOfTotal; i++)
    {
        value+=(in[i]-in[i-windowSize]);
        out[i]=value/windowSize;
    }
}
