//===================================================================
// low pass, moving average
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <stdlib.h>


static const float wav[]=
{
#include "../dim1.txt"
};


//-------------------------------------------------------------------
// lowPass
static void
lowPass(unsigned int x, const float *in,
                            float *out, const unsigned int windowSize)
{
    float value=0.0f;
    for(unsigned int i=x; i<x+windowSize; i++)
        value+=in[i];

    out[x+windowSize-1]=value/windowSize;
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    if(argc<2)
    {
        fprintf(stderr, "移動平均のウィンドウサイズを指定してください.\n");
        return -1;
    }

    size_t numOfTotal=sizeof(wav)/sizeof(wav[0]);
    size_t windowSize=atoi(argv[1]);

    float *result=new float[numOfTotal];

    for(size_t i=0; i<windowSize-1; i++)
        result[i]=0.0f;

    for(size_t x=0; x<numOfTotal-windowSize+1; x++)
        lowPass(x, wav, result, windowSize);

    for(unsigned int i=0; i<numOfTotal; i++)
        printf("%6d\n", (int)result[i]);

    delete [] result;

    return 0;
}
