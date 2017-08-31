/*
 * laplacian4 / 24bit & 32bit bmp
 *
 *  ---------In------+----------Out------------
 *                   |
 *     x0   x1   x2  |          x
 *                   |
 *      0   -1    0  |       .  .  .
 *     -1    4   -1  |       .  o  .
 *      0   -1    0  |       .  .  .
 */

__kernel void
clCode(const unsigned int width,
       const int pixelPitch,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int row0=y*rowPitch;
    int row1=row0+rowPitch;
    int row2=row1+rowPitch;

    int r=(int)(  -in[row0+((x+1)*pixelPitch)+0]

                  -in[row1+((x+0)*pixelPitch)+0]
                  +in[row1+((x+1)*pixelPitch)+0]*4
                  -in[row1+((x+2)*pixelPitch)+0]

                  -in[row2+((x+1)*pixelPitch)+0]);

    out[row1+((x+1)*pixelPitch)+0]=convert_uchar_sat(r);


    int g=(int)(  -in[row0+((x+1)*pixelPitch)+1]

                  -in[row1+((x+0)*pixelPitch)+1]
                  +in[row1+((x+1)*pixelPitch)+1]*4
                  -in[row1+((x+2)*pixelPitch)+1]

                  -in[row2+((x+1)*pixelPitch)+1]);

    out[row1+((x+1)*pixelPitch)+1]=convert_uchar_sat(g);


    int b=(int)(  -in[row0+((x+1)*pixelPitch)+2]

                  -in[row1+((x+0)*pixelPitch)+2]
                  +in[row1+((x+1)*pixelPitch)+2]*4
                  -in[row1+((x+2)*pixelPitch)+2]

                  -in[row2+((x+1)*pixelPitch)+2]);

    out[row1+((x+1)*pixelPitch)+2]=convert_uchar_sat(b);
}
