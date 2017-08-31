//===================================================================
// search (C++)
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "../../Cexp/bitmapStruct.h"
#include "../../Cexp/Cbmp.h"
#include "../../Cexp/Cexp.h"

#pragma pack(push, 1)

typedef struct
{
    int smlHeight;
    int smlWidth;
    int bigRowPitch;
    int smlRowPitch;
    int pixelPitch;
    int sumArrayWidth;
    int sumArrayHeight;
} paramType;

#pragma pack(pop)

//-------------------------------------------------------------------
// search image each position
static void
search(const int x, const int y,
       const paramType *p,
       const unsigned char* inBigImg,
       const unsigned char* inSmlImg,
       int* sumArray)
{
    int bigOffset=(y*p->bigRowPitch)+(x*p->pixelPitch);
    int smlOffset=0;

    int sum=0;
    for(int dy=0; dy<p->smlHeight; dy++)
    {
        for(int dx=0; dx<(p->smlWidth)*(p->pixelPitch); dx++)
        {
            sum+=abs(inBigImg[bigOffset+dx]
                        -inSmlImg[smlOffset+dx]);
        }
        bigOffset+=(p->bigRowPitch);
        smlOffset+=(p->smlRowPitch);
    }
    sumArray[y*p->sumArrayWidth+x]=sum;
}

//-------------------------------------------------------------------
// search image
static void
search(const Cbmp bmp[])
{
    int diff=INT_MAX, xPos=0, yPos=0;
    paramType param;

    param.smlHeight=bmp[1].mAbsHeight;              // set param
    param.smlWidth=bmp[1].mPdib->biWidth;
    param.bigRowPitch=bmp[0].mRowPitch;
    param.smlRowPitch=bmp[1].mRowPitch;
    param.pixelPitch=bmp[0].mPixelPitch;
    param.sumArrayHeight=bmp[0].mAbsHeight-param.smlHeight+1;
    param.sumArrayWidth=bmp[0].mPdib->biWidth-param.smlWidth+1;

    int *sumArray =new int[param.sumArrayHeight*param.sumArrayWidth];

    for(int y=0; y<param.sumArrayHeight; y++)
    {
        for(int x=0; x<param.sumArrayWidth; x++)
        {
            search(x, y, &param, bmp[0].mPbitmap,
                            bmp[1].mPbitmap, sumArray);
        }
    }

    //srch diff
    for(int y=0;y<param.sumArrayHeight;y++)
    {
        for(int x=0;x<param.sumArrayWidth;x++)
        {
            if(sumArray[y*param.sumArrayWidth+x]<diff)
            {
                diff=sumArray[y*param.sumArrayWidth+x];
                yPos=y;
                xPos=x;
            }
        }
    }

    delete [] sumArray;

    if(bmp[1].mPdib->biHeight>0)
        yPos=bmp[0].mAbsHeight-yPos-bmp[1].mAbsHeight;

    printf("     x=%d, y=%d, diff=%d\n", xPos, yPos, diff);
}


//-------------------------------------------------------------------
// evalate bitmap format
static int
evalBmp(const Cbmp bmp[])
{
                                                    // print image size
    fprintf(stdout, "元画像ファイル= %d x %d, %d/pixel\n",
            bmp[0].getWidth(), bmp[0].getHeight(), bmp[0].getBitsPerPixcel());

                                                    // print image size
    fprintf(stdout, "探す画像= %d x %d, %d/pixel\n",
            bmp[1].getWidth(), bmp[1].getHeight(), bmp[1].getBitsPerPixcel());

    if(bmp[0].mPdib->biWidth<bmp[1].mPdib->biWidth
        || bmp[0].mAbsHeight<bmp[1].mAbsHeight)
        throw Cexp("元画像ファイルの幅や高さが探す画像より小さい.");

    if((bmp[0].mPdib->biHeight<0 && bmp[1].mPdib->biHeight>0)
                    || (bmp[0].mPdib->biHeight>0 && bmp[1].mPdib->biHeight<0))
        throw Cexp("y軸の昇順が異なる.");

    if(bmp[0].mPixelPitch!=bmp[1].mPixelPitch)
        throw Cexp("元画像ファイルと画像のbits/pixelが異なる.");

    if(bmp[0].getBitsPerPixcel()!=24 && bmp[0].getBitsPerPixcel()!=32)
        throw Cexp("bmp must be 24/32bits per pixcel.");

    return 0;
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
                        "  <元画像ファイル>  <小さな探す画像>");

        Cbmp bmp[2];
        bmp[0].loadFromFile(argv[1]);               // load bitmap
        bmp[1].loadFromFile(argv[2]);               // load bitmap

        evalBmp(bmp);                               // evalate bitmap format

        search(bmp);                                // search image
    }
    catch (Cexp exp)
    {
        fprintf(stderr, "error: %s\n", exp.c_str());
        return -1;
    }

    return 0;
}
