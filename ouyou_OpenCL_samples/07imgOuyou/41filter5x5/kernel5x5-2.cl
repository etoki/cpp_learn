
typedef struct __attribute__ ((packed)) paramStruct
{
    int   height;
    int   width;
    int   rowPitch;
    int   pixelPitch;
} paramType;

__kernel void
clCode(__global const paramType *p,
       __global const float* filterOp,
       __global unsigned char* out,
       __global const unsigned char* in)
{
    int yx[5][5];

    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    for(int dY=0; dY<5; dY++)
        for(int dX=0; dX<5; dX++)
            yx[dY][dX]=p->rowPitch*(y+dY)+(x+dX)*p->pixelPitch;

    for(int rgb=0; rgb<3; rgb++)
    {
        float value=0.0f;
        for(int row=0; row<5; row++)
            for(int col=0; col<5; col++)
                value+=(float)in[yx[row][col]+rgb]*filterOp[(row*5)+col];

        out[yx[2][2]+rgb]=convert_uchar_sat(value);
    }
}
