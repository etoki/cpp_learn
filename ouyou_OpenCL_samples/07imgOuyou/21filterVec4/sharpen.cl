/*
 * sharpen /  only BGRA
 *
 *  ---------In------+----------Out------------
 *                   |
 *     x0   x1   x2  |          x
 *                   |
 *     -1   -1   -1  |       .  .  .
 *     -1    9   -1  |       .  o  .
 *     -1   -1   -1  |       .  .  .
 *
 *
 *      yx[0][0]    yx[0][1]    yx[0][2]
 *
 *      yx[1][0]    yx[1][1]    yx[1][2]
 *
 *      yx[2][0]    yx[2][1]    yx[2][2]
 *
 */

__kernel void
clCode(const unsigned int width,
       __global const uchar4* in,
       __global uchar4* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int4 yx[3][3];

    for(int dY=0; dY<3; dY++)
        for(int dX=0; dX<3; dX++)
            yx[dY][dX]=convert_int4(in[(y+dY)*width+(x+dX)]);

    int4 bgra=  -yx[0][0] -yx[0][1]   -yx[0][2]

                -yx[1][0] +yx[1][1]*9 -yx[1][2]

                -yx[2][0] -yx[2][1]   -yx[2][2];

    bgra.w=in[(y+1)*width+(x+1)].w;         // copy alpha ch.

    out[(y+1)*width+(x+1)]=convert_uchar4_sat(bgra);
}
