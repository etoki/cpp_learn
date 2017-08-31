//===================================================================
// effectWthreadCpp.cpp
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <windows.h>

static unsigned char* gPBitmap;
static size_t gWidth;

//---------------------------------------------------------------------------
// thread
//
void
effectThread(const unsigned int height)
{
    //for(size_t y=0; y<height; y++)
    //    for(size_t x=0 ; x<width*3 ; x++)
    //        pBitmap[y*width*3+x]=(unsigned char)(255-pBitmap[y*width*3+x]);

    for(size_t y=0; y<height; y++)
        for(size_t x=0 ; x<gWidth*3 ; x++)
            gPBitmap[y*gWidth*3+x]=(unsigned char)(255-gPBitmap[y*gWidth*3+x]);

}

//-------------------------------------------------------------------
// effect
int
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{
    HANDLE hThread;

    gPBitmap=pBitmap;
    gWidth=width;

    hThread=CreateThread(0, 0,                      //start thread
                            (LPTHREAD_START_ROUTINE)effectThread,
                                        (VOID *)height, 0, NULL);

    WaitForSingleObject(hThread, INFINITE);         // join

    return 0;
}
