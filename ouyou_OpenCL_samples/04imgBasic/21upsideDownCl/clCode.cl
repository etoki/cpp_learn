
__kernel void
clCode(const unsigned int width,
       const unsigned int height,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int rowPitch=width*3;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    unsigned int yUpper=y*rowPitch;
    unsigned int yLower=(height-y-1)*rowPitch;

    out[yUpper+x]=in[yLower+x];
}
