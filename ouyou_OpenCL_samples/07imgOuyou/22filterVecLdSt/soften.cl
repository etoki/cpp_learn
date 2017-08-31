/*
 * soften / 24bpp && 4pix boundary or 32bpp
 *
 *  ---------In---------+----------Out------------
 *                      |
 *     x0   x1  x2      |          x
 *                      |
 *      1   1   1   /   |       .  .  .
 *      1   1   1  / 9  |       .  o  .
 *      1   1   1 /     |       .  .  .
 *
 */

__kernel void
clCode(const unsigned int width,
       const int pixelPitch,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    if(pixelPitch==3)
    {                                       // 24 bpp
        int3 yx[3][3];

        for(int dY=0; dY<3; dY++)
            for(int dX=0; dX<3; dX++)
                yx[dY][dX]=convert_int3(vload3((y+dY)*width+(x+dX), in));

        int3 bgra=  +yx[0][0] +yx[0][1] +yx[0][2]

                    +yx[1][0] +yx[1][1] +yx[1][2]

                    +yx[2][0] +yx[2][1] +yx[2][2];

        bgra/=9;

        vstore3(convert_uchar3_sat(bgra), (y+1)*width+(x+1), out);
    }
    else
    {                                       // 32 bpp, BGRA
        int4 yx[3][3];

        for(int dY=0; dY<3; dY++)
            for(int dX=0; dX<3; dX++)
                yx[dY][dX]=convert_int4(vload4((y+dY)*width+(x+dX), in));

        int4 bgra=  +yx[0][0] +yx[0][1] +yx[0][2]

                    +yx[1][0] +yx[1][1] +yx[1][2]

                    +yx[2][0] +yx[2][1] +yx[2][2];

        bgra/=9;

        bgra.w=255;                         // set alpha ch.

        vstore4(convert_uchar4_sat(bgra), (y+1)*width+(x+1), out);
    }
}
