//
// Bilinear
//

typedef struct __attribute__ ((packed)) paramStruct
{
    int   inWidth;
    int   outWidth;
    int   inHeight;
    int   outHeight;
    int   inPixelPitch;
    int   outPixelPitch;
    int   inRowPitch;
    int   outRowPitch;
    int   offset;
    float delta;
} paramType;


__kernel void
affineTrans(__global const paramType *param,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

                                                    // coord. origin = center

    int outY=y-(param->outHeight/2);                // dest y coord
    int outX=x-(param->outWidth/2);                 // dest x coord

    float distance=sqrt(pow((float)outY, 2)+pow((float)outX, 2));
    distance=param->inHeight<0 ? distance : -distance; //y-‚Ì‚½‚ß‚Ìl—¶
    float radian=radians(distance*param->delta);

    float inY=(float)(outX*sin(radian)+outY*cos(radian));
    float inX=(float)(outX*cos(radian)-outY*sin(radian));

    int inFixX=(int)round(inX);                     // source y coord
    int inFixY=(int)round(inY);                     // source x coord

    float q=inY-(float)inFixY;
    float p=inX-(float)inFixX;

    inFixY+=(abs(param->inHeight)/2);               // coord. origin = upper left
    inFixX+=(param->inWidth/2);

    int dst=(y*param->outRowPitch)
                        +(x*param->outPixelPitch);

    if(inFixY>=0 && inFixY<(abs(param->inHeight)-1)
                 && inFixX>=0 && inFixX<(param->inWidth-1))
    {
        int srcX0=inFixX*param->inPixelPitch;
        int srcX1=srcX0+param->inPixelPitch;
        int srcY0=inFixY*param->inRowPitch;
        int srcY1=srcY0+param->inRowPitch;

        int src00=srcY0+srcX0;
        int src01=srcY0+srcX1;
        int src10=srcY1+srcX0;
        int src11=srcY1+srcX1;

        for(int rgb=0;rgb<3;rgb++)
            out[dst+rgb]=convert_uchar_sat(
                (int)((1.0f-q)*((1.0f-p)*(float)in[src00+rgb+param->offset]
                                      +p*(float)in[src01+rgb+param->offset])
                            +q*((1.0f-p)*(float)in[src10+rgb+param->offset]
                                      +p*(float)in[src11+rgb+param->offset]))
                                          );
    }
        // OpenCL 1.0/1.1
        //else
        //    out[dst+0]=                         // b
        //        out[dst+1]=                     // g
        //            out[dst+2]=0xFF;            // r
}
