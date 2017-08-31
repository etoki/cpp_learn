/*
 * grayscale / 32bpp BGRA only
 *
 */

const sampler_t samp=CLK_NORMALIZED_COORDS_FALSE |
                     CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;

__kernel void
grayscale(__read_only  image2d_t rgb,
          __write_only image2d_t gs)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int2 coord=(int2)(x, y);
    float4 data=read_imagef(rgb, samp, coord);

    float gsData=data.s0*0.114478f              // blue
                    +data.s1*0.586611f          // green
                        +data.s2*0.298912f;     // red

    write_imagef(gs, coord, (float4)(gsData,gsData,gsData,data.s3));
}


/*
 * laplacian8 / 32bpp BGRA only
 *
 *  ---------In------+----------Out------------
 *                   |
 *     x0   x1   x2  |          x
 *                   |
 *     -1   -1   -1  |       .  .  .
 *     -1    8   -1  |       .  o  .
 *     -1   -1   -1  |       .  .  .
 *
 */

__kernel void
filter(__read_only  image2d_t gs,
       __write_only image2d_t rgb)
{
    size_t x=get_global_id(0)+1;
    size_t y=get_global_id(1)+1;

    float4 data=(float4)0.0f;
    for(int yy=0; yy<3; yy++)
        for(int xx=0; xx<3; xx++)
        {
            int2 coord=(int2)((x-1+xx), (y-1+yy));
            if(yy==1 && xx==1)
                data+=(read_imagef(gs, samp, coord)*8.0f);
            else
                data-=read_imagef(gs, samp, coord);
        }

    data.w=1.0f;                            // set alpha ch.

    write_imagef(rgb, (int2)(x, y), data);
}
