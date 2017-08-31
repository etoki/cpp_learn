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
filter(const unsigned int width,
       const int pixelPitch,
       __global const unsigned char* gs,
       __global unsigned char* rgb)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int row0=(y+0)*width;
    int row1=(y+1)*width;
    int row2=(y+2)*width;

    int hx=(int)(

        -gs[row0+x+0]                        +gs[row0+x+2]

        -gs[row1+x+0]*2                      +gs[row1+x+2]*2

        -gs[row2+x+0]                        +gs[row2+x+2] );

    int hy=(int)(

        -gs[row0+x+0]    -gs[row0+x+1]*2    -gs[row0+x+2]



        +gs[row2+x+0]    +gs[row2+x+1]*2    +gs[row2+x+2] );

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int pos=rowPitch*(y+1)+pixelPitch*(x+1);

    rgb[pos+0]=
        rgb[pos+1]=
            rgb[pos+2]=convert_uchar_sat(hypot((float)hx,(float)hy));

}


/*
 * grayscale, in=RGB or BGRA, out=grayscale
 */
__kernel void
grayscale(const unsigned int width,
          const int pixelPitch,
          __global const unsigned char* rgb,
          __global unsigned char* gs)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int pos=y*rowPitch+x*pixelPitch;

    int data=(int)(
                (float)rgb[pos+0]*0.114478f             // blue
                    +(float)rgb[pos+1]*0.586611f        // green
                        +(float)rgb[pos+2]*0.298912f    // red
                );

    gs[(width*y)+x]=convert_uchar_sat(data);
}
