// min, max task parallel

__kernel void
minCl(__global const int* a,
      __global int* minData,
      const uint numOfTotal)
{
    for(size_t i=0; i<numOfTotal; i++)
    {
        *minData=min(*minData, a[i]);
    }
}

__kernel void
maxCl(__global const int* a,
      __global int* maxData,
      const uint numOfTotal)
{
    for(size_t i=0; i<numOfTotal; i++)
    {
        *maxData=max(*maxData, a[i]);
    }
}
