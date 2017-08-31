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
    size_t rowPitch=width*3;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    for(size_t y=0; y<height; y++)
        for(size_t x=0 ; x<width*3 ; x++)
            pBitmap[y*rowPitch+x]=(unsigned char)(255-pBitmap[y*rowPitch+x]);

    return 0;
}
