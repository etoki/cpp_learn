__kernel void
clCode(const unsigned int width,
       const unsigned int height,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    out[y*width*3+x]=(unsigned char)(255-in[y*width*3+x]);
}
