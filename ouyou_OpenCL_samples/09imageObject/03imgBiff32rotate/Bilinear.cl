//
// Bilinear
//
// coordrとinY、inXをfloatにしてsampler_tにCLK_FILTER_LINEAR
// を指定することによって、bilinear補完を読み込み時に行う。
// 書き込みはin型の座標を使う。
//

const sampler_t samp=CLK_NORMALIZED_COORDS_FALSE |
                     CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

__kernel void
affineTrans(__read_only  image2d_t in,
            __write_only image2d_t out,
            const float degree)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int width=get_image_width(in);
    int height=get_image_height(in);

    float radian=radians(degree);

    int yc=height/2;                        // y center
    int xc=width/2;                         // x center
    int outY=y-yc;                          // dest y coord
    int outX=x-xc;                          // dest x coord

    float inY=outX*sin(radian)+outY*cos(radian);
    float inX=outX*cos(radian)-outY*sin(radian);

    float2 coordr=(float2)(inX+(float)xc, inY+(float)yc);
    float4 data=read_imagef(in, samp, coordr);

    int2 coordw=(int2)(x, y);
    write_imagef(out, coordw, data);
}
