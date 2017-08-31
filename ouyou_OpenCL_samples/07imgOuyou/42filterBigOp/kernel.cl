
typedef struct __attribute__ ((packed)) paramStruct
{
    int   height;
    int   width;
    int   rowPitch;
    int   pixelPitch;
    int   opHeight;
    int   opWidth;
} paramType;

__kernel void
clCode(__global const paramType *p,
       __global const float* filterOp,
       __global unsigned char* out,
       __global const unsigned char* in)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    for(int rgb=0; rgb<3; rgb++)                    // rgb
    {
        float value=0.0f;

        for(int dy=0; dy<p->opHeight; dy++)         // y
            for(int dx=0; dx<p->opWidth; dx++)      // x
                value+=(float)in[(y+dy)*p->rowPitch
                        +(x+dx)*p->pixelPitch+rgb]
                        *filterOp[dy*p->opWidth+dx];

        out[(y+(p->opHeight/2))*p->rowPitch
                +(x+(p->opWidth/2))*p->pixelPitch
                        +rgb]=convert_uchar_sat(value);
    }
}
