//
// Bilinear
//

__kernel void
affineTrans(const int inWidth,
            const int outWidth,
            const int inHeight,
            const int outHeight,
            const int inPixelPitch,
            const int outPixelPitch,
            const int inRowPitch,
            const int outRowPitch,
            const float delta,
            const int offset,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

                                            // coord. origin = center

    int outY=y-(outHeight/2);               // dest y coord
    int outX=x-(outWidth/2);                // dest x coord

    float distance=sqrt(pow((float)outY, 2)+pow((float)outX, 2));
    distance=inHeight<0 ? distance : -distance; //y-‚Ì‚½‚ß‚Ìl—¶
    float radian=radians(distance*delta);

    float inY=(float)(outX*sin(radian)+outY*cos(radian));
    float inX=(float)(outX*cos(radian)-outY*sin(radian));

    int inFixX=(int)round(inX);             // source y coord
    int inFixY=(int)round(inY);             // source x coord

    float q=inY-(float)inFixY;
    float p=inX-(float)inFixX;

                                            // coord. origin = upper left
    inFixY+=(abs(inHeight)/2);
    inFixX+=(inWidth/2);

    int dst=(y*outRowPitch)+(x*outPixelPitch);

    if(inFixY>=0 && inFixY<(abs(inHeight)-1)
                 && inFixX>=0 && inFixX<(inWidth-1))
    {
        int srcX0=inFixX*inPixelPitch;
        int srcX1=srcX0+inPixelPitch;
        int srcY0=inFixY*inRowPitch;
        int srcY1=srcY0+inRowPitch;

        int src00=srcY0+srcX0;
        int src01=srcY0+srcX1;
        int src10=srcY1+srcX0;
        int src11=srcY1+srcX1;

        for(int rgb=0;rgb<3;rgb++)
            out[dst+rgb]=convert_uchar_sat(
                (int)((1.0f-q)*((1.0f-p)*(float)in[src00+rgb+offset]
                                      +p*(float)in[src01+rgb+offset])
                            +q*((1.0f-p)*(float)in[src10+rgb+offset]
                                      +p*(float)in[src11+rgb+offset]))
                                          );
    }
        // OpenCL 1.0/1.1
        //else
        //    out[dst+0]=                         // b
        //        out[dst+1]=                     // g
        //            out[dst+2]=0xFF;            // r
}
