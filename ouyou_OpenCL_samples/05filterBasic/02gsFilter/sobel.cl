/*
 * sobel
 *
 *      | -1  0  1 |
 *   hx=| -2  0  2 |
 *      | -1  0  1 |
 *
 *      | -1 -2 -1 |
 *   hy=|  0  0  0 |
 *      |  1  2  1 |
 *
 *          _______________
 *         /
 *  hxy=  / hx^2  +  hy^2
 *       V
 */

__kernel void
clCode(const unsigned int width,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int row0=(y+0)*width;
    int row1=(y+1)*width;
    int row2=(y+2)*width;

    int hx=(int)(

        -in[row0+x+0]                        +in[row0+x+2]

        -in[row1+x+0]*2                      +in[row1+x+2]*2

        -in[row2+x+0]                        +in[row2+x+2] );

    int hy=(int)(

        -in[row0+x+0]    -in[row0+x+1]*2    -in[row0+x+2]



        +in[row2+x+0]    +in[row2+x+1]*2    +in[row2+x+2] );

    out[row1+x+1]=convert_uchar_sat(hypot((float)hx,(float)hy));
}
