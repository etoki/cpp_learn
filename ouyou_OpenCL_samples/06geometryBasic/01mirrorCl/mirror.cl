/*
 * mirror
 *
 */
__kernel void
affineTrans(const int width,
            const int pixelPitch,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int lineOffset=rowPitch*y;
    int dstX=lineOffset+(x*pixelPitch);
    int srcX=lineOffset+((width-1)*pixelPitch-(x*pixelPitch));

    out[dstX+0]=in[srcX+0];             // b
    out[dstX+1]=in[srcX+1];             // g
    out[dstX+2]=in[srcX+2];             // r
}
