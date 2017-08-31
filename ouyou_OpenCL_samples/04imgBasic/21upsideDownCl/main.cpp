//===================================================================
// main.cpp, only 24bpp && 4Pixel boundary
//
// Ubuntu:  $ g++ -I /opt/AMDAPP/include/ -L /opt/AMDAPP/lib/x86_64/ main.cpp effectCl.cpp ../bitmap/bitmap.cpp -lOpenCL
//          ./a.out ../../Koala.bmp o.bmp
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include "../bitmap/bitmapStruct.h"

int effect(unsigned char* pBitmap, const size_t width, const size_t height);


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    pBmpInfoHdr   pDib;
    unsigned char *pBitmap;
    bmpFileHdr    bmFHdr;
    int status;

    if(argc<3)
    {
        fprintf(stderr, "入力ファイルと出力ファイルを指定してください.\n");
        return -1;
    }

    status=readBmp(argv[1], &bmFHdr, &pDib);
    if(status!=0 || pDib==0)
        return status;


    // print image info.
    fprintf(stdout, "Bitmap Info. = %d x %d, %d bits per pixel.\n",
            (int)pDib->biWidth, (int)pDib->biHeight, (int)pDib->biBitCount);

    if(pDib->biBitCount!=24 || (pDib->biWidth%4)!=0)
    {
        fprintf(stderr, "24bppで横幅が4の整数倍のみ処理対象です.\n");
        free(pDib);

        return -1;
    }

    // ポインタをビットマップ本体へ
    pBitmap=(unsigned char *)(pDib)+bmFHdr.bfOffBits-sizeof(bmpFileHdr);
    size_t height=abs(pDib->biHeight);
    size_t width=pDib->biWidth;

    effect(pBitmap, width, height);

    status=writeBmp(argv[2], &bmFHdr, pDib);
    if(status!=0)
        return status;

    free(pDib);

    return 0;
}
