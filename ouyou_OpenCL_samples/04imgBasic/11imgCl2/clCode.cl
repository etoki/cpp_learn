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

    out[y*rowPitch+x]=(unsigned char)(255-in[y*rowPitch+x]);
}
