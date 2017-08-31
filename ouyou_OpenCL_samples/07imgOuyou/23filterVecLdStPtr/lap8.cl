/*
 * laplacian8/ 24bpp & 32bpp BGRA bmp
 *
 *
 *  ---------In------+----------Out------------
 *                   |
 *     x0   x1   x2  |          x
 *                   |
 *     -1   -1   -1  |       .  .  .
 *     -1    8   -1  |       .  o  .
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
       const int pixelPitch,
       __global unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    __global unsigned char* ptr;
    int3 yx[3][3];

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    for(int dY=0; dY<3; dY++)
        for(int dX=0; dX<3; dX++)
        {
            ptr=in+(y+dY)*rowPitch+(x+dX)*pixelPitch;
            yx[dY][dX]=convert_int3(vload3(0, ptr));
        }

    int3 bgra=  -yx[0][0] -yx[0][1]   -yx[0][2]

                -yx[1][0] +yx[1][1]*8 -yx[1][2]

                -yx[2][0] -yx[2][1]   -yx[2][2];

    ptr=out+(y+1)*rowPitch+(x+1)*pixelPitch;
    vstore3(convert_uchar3_sat(bgra), 0, ptr);
}
