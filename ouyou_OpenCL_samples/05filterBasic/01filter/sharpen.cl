/*
 * sharpen / 24bit & 32bit bmp
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
 */

__kernel void
clCode(const unsigned int width,
       const int pixelPitch,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    int yx[3][3];               //“ñŽŸŒ³”z—ñ

    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    for(int dY=0; dY<3; dY++)
        for(int dX=0; dX<3; dX++)
            yx[dY][dX]=(y+dY)*rowPitch+(x+dX)*pixelPitch;

    for(int rgb=0; rgb<3; rgb++)
    {
        int data=(int)(

        -in[yx[0][0]+rgb] -in[yx[0][1]+rgb]   -in[yx[0][2]+rgb]

        -in[yx[1][0]+rgb] +in[yx[1][1]+rgb]*9 -in[yx[1][2]+rgb]

        -in[yx[2][0]+rgb] -in[yx[2][1]+rgb]   -in[yx[2][2]+rgb] );

        out[yx[1][1]+rgb]=convert_uchar_sat(data);
    }
}
