/*
 * laplacian8
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

    int data=(int)(

        -in[row0+x+0]  -in[row0+x+1]    -in[row0+x+2]

        -in[row1+x+0]  +in[row1+x+1]*8  -in[row1+x+2]

        -in[row2+x+0]  -in[row2+x+1]    -in[row2+x+2] );

    out[row1+x+1]=convert_uchar_sat(data);
}
