//===================================================================
// sum, reduction (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>


//-------------------------------------------------------------------
// main
int
main()
{
    int a[10000], sum;
    size_t numOfTotal=sizeof(a)/sizeof(a[0]);


    for(size_t i=0; i<numOfTotal; i++)              // initialize array
        a[i]=i;

    sum=0;                                          // calc.
    for(size_t i=0; i<numOfTotal; i++)
    {
        sum+=a[i];
    }

    printf("sum=%d\n", sum);                        // print

    return 0;
}
