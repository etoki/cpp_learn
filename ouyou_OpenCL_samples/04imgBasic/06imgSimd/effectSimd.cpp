//===================================================================
// effectSimd.cpp, only Intel(TM) format(x86).
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
//===================================================================


//-------------------------------------------------------------------
// effect
int
effect(unsigned char* pBitmap, const size_t width, const size_t height)
{

    _asm
    {
        mov     esi,  pBitmap   /* esi = src                    */

        mov     eax, height
        mov     edx, width
        mul     edx             /* eax=height * width           */
        mov     edx, 3
        mul     edx             /* eax=height * width*3         */
        shr     eax, 4          /* eax=(height * width*3)/16    */
        mov     ecx, eax        /* loop counter                 */

loopLbl:
        movdqu  xmm1, [esi]     /* load 16 bytes                */

        pcmpeqb xmm0, xmm0      /* set all bits                 */
        psubb   xmm0, xmm1      /* neg                          */

        movdqu  [esi], xmm0     /* save it                      */
        lea     esi,  [esi+16]  /* next address                 */

        loop    loopLbl         /* dec ecx and jump             */
    }

    return 0;
}
