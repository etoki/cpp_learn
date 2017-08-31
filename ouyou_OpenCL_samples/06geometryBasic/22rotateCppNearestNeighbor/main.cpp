//===================================================================
// rotate, NearestNeighbor (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"

#ifndef M_PI
#define M_PI 3.14159265358979f              // pi
#endif //M_PI

#define radian2degree(a) ((a)/M_PI*180.0)   // radian to degree
#define degree2radian(a) ((a)/180.0*M_PI)   // degree to radian


//-------------------------------------------------------------------
// if C99, use roundf
static float inline
round_f(const float x)
{
    return x>0.0f ? (float)floor(x+0.5f) : (float)(-1.0*floor(fabs(x)+0.5));
}

//-------------------------------------------------------------------
// rotate(cpp)
static void
affineTrans(const Cbmp* bmp, const float inDegree)
{
    unsigned int datasize=bmp->mImageSize;
    unsigned char* orgImg=new unsigned char[datasize];

    memcpy(orgImg, bmp->mPbitmap, datasize);        // ピクセルデータの保存

    unsigned char* in=orgImg;
    unsigned char* out=bmp->mPbitmap;

    float degree=bmp->getHeight()<0 ? inDegree : 360.0f-inDegree;
    float radian=(float)degree2radian(degree);      // 反時計方向

    int yc=bmp->mAbsHeight/2;                       // y center
    int xc=bmp->mPdib->biWidth/2;                   // x center

                                                    // dest y coord
    for(int outY=-yc; outY<bmp->mAbsHeight-yc; outY++)
    {
                                                    // dest x coord
        for(int outX=-xc; outX<bmp->mPdib->biWidth-xc; outX++)
        {
            float inY=(float)(outX*sin(radian)+outY*cos(radian));
            float inX=(float)(outX*cos(radian)-outY*sin(radian));

            int inFixY=(int)round_f(inY);   // source y coord
            int inFixX=(int)round_f(inX);   // source x coord

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
                        "  <入力ファイル>  <出力ファイル>  <回転角度>");

        float degree=(float)atof(argv[3]);

        Cbmp bmp;
        bmp.loadFromFile(argv[1]);                  // load bitmap

        if(bmp.getBitsPerPixcel()!=24 && bmp.getBitsPerPixcel()!=32)
            throw Cexp("bmp must be 24/32bits per pixcel.");

                                                    // print image size
        fprintf(stdout, "ビットマップサイズ= %d x %d, %d/pixel\n",
                bmp.getWidth(), bmp.getHeight(), bmp.getBitsPerPixcel());

        affineTrans(&bmp, degree);                  // rotate

        bmp.saveToFile(argv[2]);                    // save bitmap
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
