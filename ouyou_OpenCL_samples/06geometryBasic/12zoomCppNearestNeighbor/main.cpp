//===================================================================
// zoom, NearestNeighbor (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"


//-------------------------------------------------------------------
// if C99, use roundf
static float inline
round_f(const float x)
{
        //if(x>0.0)
        //    return floor(x + 0.5);
        //else
        //    return -1.0 * floor(fabs(x) + 0.5);

    return x>0.0f ? (float)floor(x+0.5f) : (float)(-1.0*floor(fabs(x)+0.5));
}

//-------------------------------------------------------------------
// zoom(cpp)
static void
affineTrans(const Cbmp* bmp, const float scale)
{
    unsigned int datasize=bmp->mImageSize;
    unsigned char* orgImg=new unsigned char[datasize];

    memcpy(orgImg, bmp->mPbitmap, datasize);        // ピクセルデータの保存

    unsigned char* in=orgImg;
    unsigned char* out=bmp->mPbitmap;


    int yc=bmp->mAbsHeight/2;                       // y center
    int xc=bmp->mPdib->biWidth/2;                   // x center

                                                    // dest y coord
    for(int outY=-yc; outY<bmp->mAbsHeight-yc; outY++)
    {
                                                    // dest x coord
        for(int outX=-xc; outX<bmp->mPdib->biWidth-xc; outX++)
        {
            int inFixY=(int)round_f(outY/scale);    // source y coord
            int inFixX=(int)round_f(outX/scale);    // source x coord

            int dstX=(outX+xc)*bmp->mPixelPitch;
            int dstY=(outY+yc)*bmp->mRowPitch;
            int dst=dstY+dstX;

            if((inFixY>=-yc) && (inFixY<yc) &&
                        (inFixX>=-xc) && (inFixX<xc))
            {
                int srcX=(inFixX+xc)*bmp->mPixelPitch;
                int srcY=(inFixY+yc)*bmp->mRowPitch;
                int src=srcY+srcX;

                out[dst+0]=in[src+0];       // b
                out[dst+1]=in[src+1];       // g
                out[dst+2]=in[src+2];       // r
            }
            else
            {
                out[dst+0]=                 // b
                    out[dst+1]=             // g
                        out[dst+2]=0xFF;    // r
            }
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
        if(argc!=4)
            throw Cexp( "引数に以下の値を指定してください.\n" \
                        "  <入力ファイル>  <出力ファイル>  <スケール>");

        float scale=(float)atof(argv[3]);

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        affineTrans(&bmp, scale);                   // zoom

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
