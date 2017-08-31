//===================================================================
// zoom, Bilinear (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"

template<class T> static T min(T a, T b);
template<class T> static T max(T a, T b);
static unsigned char convert_uchar_sat(int val);

//-------------------------------------------------------------------
// if C99, use roundf
static float inline
round_f(const float x)
{
    return x>0.0f ? (float)floor(x+0.5f) : (float)(-1.0*floor(fabs(x)+0.5));
}

//-------------------------------------------------------------------
// zoom(cpp)
static void
affineTrans(const Cbmp* bmp, const float scale)
{
    int r, g, b;
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
            float inY=outY/scale;
            float inX=outX/scale;

            int inFixY=(int)round_f(inY);           // source y coord
            int inFixX=(int)round_f(inX);           // source x coord

            float q=inY-(float)inFixY;
            float p=inX-(float)inFixX;

            inFixX+=xc;
            inFixY+=yc;
            int oX=outX+xc;
            int oY=outY+yc;

            int dstX=oX*bmp->mPixelPitch;
            int dstY=oY*bmp->mRowPitch;
            int dst=dstY+dstX;

            if(inFixY>=0 && inFixY<(bmp->mAbsHeight-1)
                         && inFixX>=0 && inFixX<(bmp->mPdib->biWidth-1))
            {
                int srcX0=inFixX*bmp->mPixelPitch;
                int srcX1=srcX0+bmp->mPixelPitch;
                int srcY0=inFixY*bmp->mRowPitch;
                int srcY1=srcY0+bmp->mRowPitch;

                int src00=srcY0+srcX0;
                int src01=srcY0+srcX1;
                int src10=srcY1+srcX0;
                int src11=srcY1+srcX1;

                b=(int)((1.0f-q)*((1.0f-p)*(float)in[src00+0]
                                        +p*(float)in[src01+0])
                              +q*((1.0f-p)*(float)in[src10+0]
                                        +p*(float)in[src11+0]));

                g=(int)((1.0f-q)*((1.0f-p)*(float)in[src00+1]
                                        +p*(float)in[src01+1])
                              +q*((1.0f-p)*(float)in[src10+1]
                                        +p*(float)in[src11+1]));

                r=(int)((1.0f-q)*((1.0f-p)*(float)in[src00+2]
                                        +p*(float)in[src01+2])
                              +q*((1.0f-p)*(float)in[src10+2]
                                        +p*(float)in[src11+2]));

                out[dst+0]=convert_uchar_sat(b);    // b
                out[dst+1]=convert_uchar_sat(g);    // g
                out[dst+2]=convert_uchar_sat(r);    // r
            }
            else
            {
                out[dst+0]=                         // b
                    out[dst+1]=                     // g
                        out[dst+2]=0xFF;            // r
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


//-------------------------------------------------------------------
// min(a,b)
template<class T>
static T
min(T a, T b)
{
    return ( a < b ) ? a : b;
}

//-------------------------------------------------------------------
// max(a,b)
template<class T>
static T
max(T a, T b)
{
    return ( a > b ) ? a : b;
}

//-------------------------------------------------------------------
// unsigned char satulete(val)
static unsigned char
convert_uchar_sat(int val)
{
    return max(0, min(val, 0xff));
}
