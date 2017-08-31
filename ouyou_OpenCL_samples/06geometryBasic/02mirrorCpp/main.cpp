//===================================================================
// mirror
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <string.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"


//-------------------------------------------------------------------
// mirror(Cpp)
static void
affineTrans(const char* clName, const Cbmp* bmp)
{
    unsigned int datasize=bmp->mImageSize;
    unsigned char* orgImg=new unsigned char[datasize];
    memcpy(orgImg, bmp->mPbitmap, datasize);    // ピクセルデータの保存

    unsigned char* in=orgImg;
    unsigned char* out=bmp->mPbitmap;
    for(size_t y=0;y<(size_t)bmp->mAbsHeight;y++)
    {
        for(size_t x=0;x<(size_t)bmp->mPdib->biWidth;x++)
        {
            int rowOffset=bmp->mRowPitch*y;
            int dstX=rowOffset+(x*bmp->mPixelPitch);
            int srcX=rowOffset+((bmp->mPdib->biWidth-1)
                            *bmp->mPixelPitch-(x*bmp->mPixelPitch));

            out[dstX+0]=in[srcX+0];             // b
            out[dstX+1]=in[srcX+1];             // g
            out[dstX+2]=in[srcX+2];             // r
        }
    }

    delete [] orgImg;
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    try
    {
        if(argc!=3)
            throw Cexp( "引数に以下の値を指定してください.\n" \
                        "  <入力ファイル>  <出力ファイル>");

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        affineTrans(argv[3], &bmp);                 // mirror

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
