//
// Nearest neighbor
//

__kernel void
affineTrans(const int width,
            const int height,
            const int pixelPitch,
            const float degree,
            __global const unsigned char* in,
            __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    float radian=radians(degree);           // ”½ŽžŒv•ûŒü

    int yc=height/2;                        // y center
    int xc=width/2;                         // x center
    int outY=y-yc;                          // dest y coord
    int outX=x-xc;                          // dest x coord

    float inY=(float)(outX*sin(radian)+outY*cos(radian));
    float inX=(float)(outX*cos(radian)-outY*sin(radian));

    int inFixX=(int)round(inX);             // source y coord
    int inFixY=(int)round(inY);             // source x coord

    int rowPitch=width*pixelPitch;
    if(rowPitch%4)
        rowPitch=rowPitch+(4-(rowPitch%4));

    int dst=y*rowPitch+x*pixelPitch;

    if((inFixY>=-yc) && (inFixY<yc) && (inFixX>=-xc) && (inFixX<xc))
    {
        int src=(inFixY+yc)*rowPitch+(inFixX+xc)*pixelPitch;

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
