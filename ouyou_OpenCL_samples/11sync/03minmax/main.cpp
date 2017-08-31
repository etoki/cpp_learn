//===================================================================
// min, max (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

template<class T> static T min(T a, T b);
template<class T> static T max(T a, T b);

//-------------------------------------------------------------------
// main
int
main()
{
    int a[100000], minData=INT_MAX, maxData=INT_MIN;
    size_t numOfTotal=sizeof(a)/sizeof(a[0]);

    // initialize array
    for(size_t i=0; i<numOfTotal; i++)
        a[i]=(int)rand();


    // min / max
    for(size_t i=0; i<numOfTotal; i++)
    {
        minData=min(minData, a[i]);
        maxData=max(maxData, a[i]);
    }

    // print
    printf("max=%d, min=%d\n", maxData, minData);

    return 0;
}

//-------------------------------------------------------------------
// a, b の小さい方を返す
template<class T>
static T
min(T a, T b)
{
    return ( a < b ) ? a : b;
}

//-------------------------------------------------------------------
// a, b の大きい方を返す
template<class T>
static T
max(T a, T b)
{
    return ( a > b ) ? a : b;
}
