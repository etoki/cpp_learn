
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
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    float r=  (float)in[p->rowPitch*(y+0)+(x+0)*p->pixelPitch+0]*filterOp[(0*5)+0]
            + (float)in[p->rowPitch*(y+0)+(x+1)*p->pixelPitch+0]*filterOp[(0*5)+1]
            + (float)in[p->rowPitch*(y+0)+(x+2)*p->pixelPitch+0]*filterOp[(0*5)+2]
            + (float)in[p->rowPitch*(y+0)+(x+3)*p->pixelPitch+0]*filterOp[(0*5)+3]
            + (float)in[p->rowPitch*(y+0)+(x+4)*p->pixelPitch+0]*filterOp[(0*5)+4]

            + (float)in[p->rowPitch*(y+1)+(x+0)*p->pixelPitch+0]*filterOp[(1*5)+0]
            + (float)in[p->rowPitch*(y+1)+(x+1)*p->pixelPitch+0]*filterOp[(1*5)+1]
            + (float)in[p->rowPitch*(y+1)+(x+2)*p->pixelPitch+0]*filterOp[(1*5)+2]
            + (float)in[p->rowPitch*(y+1)+(x+3)*p->pixelPitch+0]*filterOp[(1*5)+3]
            + (float)in[p->rowPitch*(y+1)+(x+4)*p->pixelPitch+0]*filterOp[(1*5)+4]

            + (float)in[p->rowPitch*(y+2)+(x+0)*p->pixelPitch+0]*filterOp[(2*5)+0]
            + (float)in[p->rowPitch*(y+2)+(x+1)*p->pixelPitch+0]*filterOp[(2*5)+1]
            + (float)in[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+0]*filterOp[(2*5)+2]
            + (float)in[p->rowPitch*(y+2)+(x+3)*p->pixelPitch+0]*filterOp[(2*5)+3]
            + (float)in[p->rowPitch*(y+2)+(x+4)*p->pixelPitch+0]*filterOp[(2*5)+4]

            + (float)in[p->rowPitch*(y+3)+(x+0)*p->pixelPitch+0]*filterOp[(3*5)+0]
            + (float)in[p->rowPitch*(y+3)+(x+1)*p->pixelPitch+0]*filterOp[(3*5)+1]
            + (float)in[p->rowPitch*(y+3)+(x+2)*p->pixelPitch+0]*filterOp[(3*5)+2]
            + (float)in[p->rowPitch*(y+3)+(x+3)*p->pixelPitch+0]*filterOp[(3*5)+3]
            + (float)in[p->rowPitch*(y+3)+(x+4)*p->pixelPitch+0]*filterOp[(3*5)+4]

            + (float)in[p->rowPitch*(y+4)+(x+0)*p->pixelPitch+0]*filterOp[(4*5)+0]
            + (float)in[p->rowPitch*(y+4)+(x+1)*p->pixelPitch+0]*filterOp[(4*5)+1]
            + (float)in[p->rowPitch*(y+4)+(x+2)*p->pixelPitch+0]*filterOp[(4*5)+2]
            + (float)in[p->rowPitch*(y+4)+(x+3)*p->pixelPitch+0]*filterOp[(4*5)+3]
            + (float)in[p->rowPitch*(y+4)+(x+4)*p->pixelPitch+0]*filterOp[(4*5)+4];

    out[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+0]=convert_uchar_sat(r);


    float g=  (float)in[p->rowPitch*(y+0)+(x+0)*p->pixelPitch+1]*filterOp[(0*5)+0]
            + (float)in[p->rowPitch*(y+0)+(x+1)*p->pixelPitch+1]*filterOp[(0*5)+1]
            + (float)in[p->rowPitch*(y+0)+(x+2)*p->pixelPitch+1]*filterOp[(0*5)+2]
            + (float)in[p->rowPitch*(y+0)+(x+3)*p->pixelPitch+1]*filterOp[(0*5)+3]
            + (float)in[p->rowPitch*(y+0)+(x+4)*p->pixelPitch+1]*filterOp[(0*5)+4]

            + (float)in[p->rowPitch*(y+1)+(x+0)*p->pixelPitch+1]*filterOp[(1*5)+0]
            + (float)in[p->rowPitch*(y+1)+(x+1)*p->pixelPitch+1]*filterOp[(1*5)+1]
            + (float)in[p->rowPitch*(y+1)+(x+2)*p->pixelPitch+1]*filterOp[(1*5)+2]
            + (float)in[p->rowPitch*(y+1)+(x+3)*p->pixelPitch+1]*filterOp[(1*5)+3]
            + (float)in[p->rowPitch*(y+1)+(x+4)*p->pixelPitch+1]*filterOp[(1*5)+4]

            + (float)in[p->rowPitch*(y+2)+(x+0)*p->pixelPitch+1]*filterOp[(2*5)+0]
            + (float)in[p->rowPitch*(y+2)+(x+1)*p->pixelPitch+1]*filterOp[(2*5)+1]
            + (float)in[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+1]*filterOp[(2*5)+2]
            + (float)in[p->rowPitch*(y+2)+(x+3)*p->pixelPitch+1]*filterOp[(2*5)+3]
            + (float)in[p->rowPitch*(y+2)+(x+4)*p->pixelPitch+1]*filterOp[(2*5)+4]

            + (float)in[p->rowPitch*(y+3)+(x+0)*p->pixelPitch+1]*filterOp[(3*5)+0]
            + (float)in[p->rowPitch*(y+3)+(x+1)*p->pixelPitch+1]*filterOp[(3*5)+1]
            + (float)in[p->rowPitch*(y+3)+(x+2)*p->pixelPitch+1]*filterOp[(3*5)+2]
            + (float)in[p->rowPitch*(y+3)+(x+3)*p->pixelPitch+1]*filterOp[(3*5)+3]
            + (float)in[p->rowPitch*(y+3)+(x+4)*p->pixelPitch+1]*filterOp[(3*5)+4]

            + (float)in[p->rowPitch*(y+4)+(x+0)*p->pixelPitch+1]*filterOp[(4*5)+0]
            + (float)in[p->rowPitch*(y+4)+(x+1)*p->pixelPitch+1]*filterOp[(4*5)+1]
            + (float)in[p->rowPitch*(y+4)+(x+2)*p->pixelPitch+1]*filterOp[(4*5)+2]
            + (float)in[p->rowPitch*(y+4)+(x+3)*p->pixelPitch+1]*filterOp[(4*5)+3]
            + (float)in[p->rowPitch*(y+4)+(x+4)*p->pixelPitch+1]*filterOp[(4*5)+4];

    out[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+1]=convert_uchar_sat(g);


    float b=  (float)in[p->rowPitch*(y+0)+(x+0)*p->pixelPitch+2]*filterOp[(0*5)+0]
            + (float)in[p->rowPitch*(y+0)+(x+1)*p->pixelPitch+2]*filterOp[(0*5)+1]
            + (float)in[p->rowPitch*(y+0)+(x+2)*p->pixelPitch+2]*filterOp[(0*5)+2]
            + (float)in[p->rowPitch*(y+0)+(x+3)*p->pixelPitch+2]*filterOp[(0*5)+3]
            + (float)in[p->rowPitch*(y+0)+(x+4)*p->pixelPitch+2]*filterOp[(0*5)+4]

            + (float)in[p->rowPitch*(y+1)+(x+0)*p->pixelPitch+2]*filterOp[(1*5)+0]
            + (float)in[p->rowPitch*(y+1)+(x+1)*p->pixelPitch+2]*filterOp[(1*5)+1]
            + (float)in[p->rowPitch*(y+1)+(x+2)*p->pixelPitch+2]*filterOp[(1*5)+2]
            + (float)in[p->rowPitch*(y+1)+(x+3)*p->pixelPitch+2]*filterOp[(1*5)+3]
            + (float)in[p->rowPitch*(y+1)+(x+4)*p->pixelPitch+2]*filterOp[(1*5)+4]

            + (float)in[p->rowPitch*(y+2)+(x+0)*p->pixelPitch+2]*filterOp[(2*5)+0]
            + (float)in[p->rowPitch*(y+2)+(x+1)*p->pixelPitch+2]*filterOp[(2*5)+1]
            + (float)in[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+2]*filterOp[(2*5)+2]
            + (float)in[p->rowPitch*(y+2)+(x+3)*p->pixelPitch+2]*filterOp[(2*5)+3]
            + (float)in[p->rowPitch*(y+2)+(x+4)*p->pixelPitch+2]*filterOp[(2*5)+4]

            + (float)in[p->rowPitch*(y+3)+(x+0)*p->pixelPitch+2]*filterOp[(3*5)+0]
            + (float)in[p->rowPitch*(y+3)+(x+1)*p->pixelPitch+2]*filterOp[(3*5)+1]
            + (float)in[p->rowPitch*(y+3)+(x+2)*p->pixelPitch+2]*filterOp[(3*5)+2]
            + (float)in[p->rowPitch*(y+3)+(x+3)*p->pixelPitch+2]*filterOp[(3*5)+3]
            + (float)in[p->rowPitch*(y+3)+(x+4)*p->pixelPitch+2]*filterOp[(3*5)+4]

            + (float)in[p->rowPitch*(y+4)+(x+0)*p->pixelPitch+2]*filterOp[(4*5)+0]
            + (float)in[p->rowPitch*(y+4)+(x+1)*p->pixelPitch+2]*filterOp[(4*5)+1]
            + (float)in[p->rowPitch*(y+4)+(x+2)*p->pixelPitch+2]*filterOp[(4*5)+2]
            + (float)in[p->rowPitch*(y+4)+(x+3)*p->pixelPitch+2]*filterOp[(4*5)+3]
            + (float)in[p->rowPitch*(y+4)+(x+4)*p->pixelPitch+2]*filterOp[(4*5)+4];

    out[p->rowPitch*(y+2)+(x+2)*p->pixelPitch+2]=convert_uchar_sat(b);
}
