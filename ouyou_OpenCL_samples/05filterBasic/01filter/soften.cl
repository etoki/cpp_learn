/*
 * soften / 24bit & 32bit bmp
 *
 *  ---------In---------+----------Out------------
 *                      |
 *     x0   x1  x2      |          x
 *                      |
 *      1   1   1   /   |       .  .  .
 *      1   1   1  / 9  |       .  o  .
 *      1   1   1 /     |       .  .  .
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

    for(int rgb=0; rgb<3; rgb++)
    {
        int data=0;
        for(int dY=0; dY<3; dY++)
            for(int dX=0; dX<3; dX++)
                data+=(int)in[(y+dY)*rowPitch+(x+dX)*pixelPitch+rgb];

        data/=9;

        out[rowPitch*(y+1)+(x+1)*pixelPitch+rgb]=data;
    }
}
