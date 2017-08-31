//===================================================================
// read/write bitmap
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>           // for SIDBA

#include "bitmapStruct.h"

static int  mBitmapSize;


//-------------------------------------------------------------------
// read bitmap file header
static int
readHeader(FILE* fp, const pBmpFileHdr pBmFHdr)
{
    if(fread(pBmFHdr, sizeof(bmpFileHdr), 1, fp)!=1)
    {
        fprintf(stderr, "error: failed fread.\n");
        return -1;
    }

    if(pBmFHdr->bfType != 'M'*256+'B')
    {
        fprintf(stderr, "error: file is't bitmap file.\n");
        return -2;
    }
    return 0;
}


//-------------------------------------------------------------------
// write bitmap file header
static int
writeHeader(FILE* fp, const pBmpFileHdr pBmFHdr)
{
    if(fwrite(pBmFHdr, sizeof(bmpFileHdr), 1, fp)!=1)
    {
        fprintf(stderr, "error: failed fwrite.\n");
        return -1;
    }
    return 0;
}


//-------------------------------------------------------------------
// read bitmap body
static int
readBody(FILE* fp, const pBmpFileHdr pBmFHdr, const pBmpInfoHdr pDib)
{
    //int bitmapSize;                               // for SIDBA

    //bitmapSize=pBmFHdr->bfSize-sizeof(bmpFileHdr);// get size

    //if(fread(pDib , bitmapSize, 1, fp)!=1)        // read image
    if(fread(pDib , mBitmapSize, 1, fp)!=1)         // read image
    {
        fprintf(stderr, "error: failed fread.\n");
        return -1;
    }
    if(pDib->biBitCount!=24 && pDib->biBitCount!=32)// 24/32 bits bitmap ?
    {
        fprintf(stderr, "error：no 24bpp nor 32bpp bitmap.\n");
        return -2;
    }
    return 0;
}


//-------------------------------------------------------------------
// write bitmap body
static int
writeBody(FILE* fp, const pBmpFileHdr pBmFHdr, const pBmpInfoHdr pDib)
{
    //int bitmapSize;

    //bitmapSize=pBmFHdr->bfSize-sizeof(bmpFileHdr);// get size

    //if(fwrite(pDib , bitmapSize, 1, fp)!=1)       // write body
    if(fwrite(pDib , mBitmapSize, 1, fp)!=1)        // write body
    {
        fprintf(stderr, "error: failed fwrite.\n");
        return -1;
    }
    return 0;
}


//-------------------------------------------------------------------
// read bitmap
int
readBmp(char* fName, const pBmpFileHdr pBmFHdr, pBmpInfoHdr *pDib)
{
    FILE*       fp;
    struct stat statbuf;                            // for SIDBA

    if((fp=fopen(fName, "rb"))==NULL)               // open input file
    {
        fprintf(stderr, "error: failed fopen.\n");
        return -1;
    }

    if(readHeader(fp, pBmFHdr)!=0)                  // read header
    {
        fclose(fp);                                 // close file
        return -1;
    }

    if(stat(fName, &statbuf)!=0)                    // for SIDBA
    {                                               // for SIDBA
        fprintf(stderr, "error: failed stat.\n");   // for SIDBA
        return -1;                                  // for SIDBA
    }                                               // for SIDBA

    //bitmapSize=pBmFHdr->bfSize-sizeof(bmpFileHdr);// get size of image
    mBitmapSize=statbuf.st_size-sizeof(bmpFileHdr); // for SIDBA
    //*pDib=(bmpInfoHdr *)malloc(bitmapSize);       // alloc memory
    *pDib=(bmpInfoHdr *)malloc(mBitmapSize);        // alloc memory

    if(readBody(fp, pBmFHdr, *pDib)!=0)             // read image
        return -1;

    fclose(fp);                                     // close file

    return 0;
}


//-------------------------------------------------------------------
// write bitmap
int
writeBmp(char* fName, const pBmpFileHdr pBmFHdr, const pBmpInfoHdr pDib)
{
    FILE* fp;
                                                    // open output file
    if((fp=fopen(fName, "wb"))!=0)
        if(writeHeader(fp, pBmFHdr)==0)             // write header
            writeBody(fp, pBmFHdr, pDib);           // write image
    else
        fprintf(stderr, "error: failed fopen.\n");

    fclose(fp);                                     // close file

    return 0;
}
