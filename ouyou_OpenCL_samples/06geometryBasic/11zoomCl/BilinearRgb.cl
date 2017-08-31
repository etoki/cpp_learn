//
// BilinearAreduce static step
//
__kernel void
affineTrans(const int width,
            const int height,
            const int pixelPitch,
            const float scale,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int yc=height/2;                        // y center
    int xc=width/2;                         // x center
    int outY=y-yc;                          // dest y coord
    int outX=x-xc;                          // dest x coord

    float inY=outY/scale;
    float inX=outX/scale;

    int inFixY=(int)round(outY/scale);      // source y coord
    int inFixX=(int)round(outX/scale);      // source x coord

    float q=inY-(float)inFixY;
    float p=inX-(float)inFixX;

    inFixX+=xc;
    inFixY+=yc;

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int dst=y*rowPitch+x*pixelPitch;

    if(inFixY>=0 && inFixY<(height-1)
                 && inFixX>=0 && inFixX<(width-1))
    {
        int srcX0=inFixX*pixelPitch;
        int srcX1=srcX0+pixelPitch;
        int srcY0=inFixY*rowPitch;
        int srcY1=srcY0+rowPitch;


        int src00=srcY0+srcX0;
        int src01=srcY0+srcX1;
        int src10=srcY1+srcX0;
        int src11=srcY1+srcX1;

        for(int rgb=0;rgb<3;rgb++)
            out[dst+rgb]=convert_uchar_sat(

                    (int)((1.0f-q)*((1.0f-p)*(float)in[src00+rgb]
                                +p*(float)in[src01+rgb])
                      +q*((1.0f-p)*(float)in[src10+rgb]
                                +p*(float)in[src11+rgb]))

            );
    }
        // OpenCL 1.0/1.1
        //else
        //    out[dst+0]=                         // b
        //        out[dst+1]=                     // g
        //            out[dst+2]=0xFF;            // r
}
