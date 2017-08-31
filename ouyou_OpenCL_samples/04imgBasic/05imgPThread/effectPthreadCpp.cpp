//===================================================================
// effectPthreadCpp.cpp
//
//    $ g++ -pthread ../01imgCl/main.cpp ../bitmap/bitmap.cpp effectPthreadCpp.cpp
//    $ ./a.out a.bmp b.bmp
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static unsigned char* gPBitmap;
static size_t gWidth;

//---------------------------------------------------------------------------
// thread
//
void*
effectThread(void *height)
{
    //for(size_t y=0; y<height; y++)
    //    for(size_t x=0 ; x<width*3 ; x++)
    //        pBitmap[y*width*3+x]=(unsigned char)(255-pBitmap[y*width*3+x]);

    int* myHight=(int*)height;

    for(size_t y=0; y<(size_t)*myHight; y++)
        for(size_t x=0 ; x<gWidth*3 ; x++)
            gPBitmap[y*gWidth*3+x]=(unsigned char)(255-gPBitmap[y*gWidth*3+x]);

     return 0;
}

//-------------------------------------------------------------------
// effect
int
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{
    int iret;
    pthread_t thread;

    gPBitmap=pBitmap;
    gWidth=width;

    iret=pthread_create( &thread, NULL, effectThread, (void*)&height);

    pthread_join(thread, NULL);         // join

    return 0;
}
