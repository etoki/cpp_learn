//
// Nearest neighbor, pitchを内部で計算
//

__kernel void
affineTrans(const int inWidth,
            const int outWidth,
            const int inHeight,
            const int outHeight,
            const int inPixelPitch,
            const int outPixelPitch,
            const float degree,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int inRowPitch=inWidth*inPixelPitch;
    if(inRowPitch%4)
        inRowPitch=inRowPitch+(4-(inRowPitch%4));

    int outRowPitch=outWidth*outPixelPitch;
    if(outRowPitch%4)
        outRowPitch=outRowPitch+(4-(outRowPitch%4));

    float radian=radians(degree);

                                            // coord. origin = center

    int outY=y-(outHeight/2);               // dest y coord
    int outX=x-(outWidth/2);                // dest x coord

    float inY=(float)(outX*sin(radian)+outY*cos(radian));
    float inX=(float)(outX*cos(radian)-outY*sin(radian));

    int inFixX=(int)round(inX);             // source y coord
    int inFixY=(int)round(inY);             // source x coord


                                            // coord. origin = upper left
    inFixY+=(inHeight/2);
    inFixX+=(inWidth/2);

    int dst=(y*outRowPitch)+(x*outPixelPitch);

    if(inFixY>=0 && inFixY<(inHeight-1)
                 && inFixX>=0 && inFixX<(inWidth-1))
    {
        int src=(inFixY*inRowPitch)+(inFixX*inPixelPitch);

        out[dst+0]=in[src+0];               // b
        out[dst+1]=in[src+1];               // g
        out[dst+2]=in[src+2];               // r
    }
        // OpenCL 1.0/1.1
        //else
        //    out[dst+0]=                         // b
        //        out[dst+1]=                     // g
        //            out[dst+2]=0xFF;            // r
}
