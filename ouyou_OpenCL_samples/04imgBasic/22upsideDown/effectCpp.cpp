//===================================================================
// upside down
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
    int rowPitch=width*3;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    for(unsigned int y=0; y<height/2; y++)
    {
        unsigned int yUpper=y*rowPitch;
        unsigned int yLower=(height-y-1)*rowPitch;

        for(unsigned int x=0 ; x<width*3 ; x++)
        {
            unsigned char tmp=pBitmap[yUpper+x];
            pBitmap[yUpper+x]=pBitmap[yLower+x];
            pBitmap[yLower+x]=tmp;
        }
    }
    return 0;
}
