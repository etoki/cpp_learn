//===================================================================
// effectCpp.cpp
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>

//-------------------------------------------------------------------
// effect
int
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{
    //for(size_t y=0; y<height; y++)
    //    for(size_t x=0 ; x<width*3 ; x++)
    //        pBitmap[y*width*3+x]=(unsigned char)(255-pBitmap[y*width*3+x]);

    int x, y;

    #pragma omp parallel for private(x)
    for(y=0; y<(int)height; y++)
        for(x=0 ; x<(int)(width*3) ; x++)
            pBitmap[y*width*3+x]=(unsigned char)(255-pBitmap[y*width*3+x]);

    return 0;
}
