//===================================================================
//  bitmap structs
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#ifndef __BITMAPSTRUCT__
#define __BITMAPSTRUCT__

#define SAFE_FREE(p)      if(p!=0) { free(p);   p=0; }
#define abs(a)            ((a) < 0 ? - (a) : (a))

#pragma pack(push, 1)

typedef struct
{
    unsigned short  bfType;
    unsigned int    bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned int    bfOffBits;
}
bmpFileHdr, *pBmpFileHdr;

typedef struct
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
}
bmpInfoHdr, *pBmpInfoHdr;


#pragma pack(pop)

int readBmp(char* fName, const pBmpFileHdr pBmFHdr, pBmpInfoHdr *pDib);
int writeBmp(char* fName, const pBmpFileHdr pBmFHdr, const pBmpInfoHdr pDib);

#endif // __BITMAPSTRUCT__
