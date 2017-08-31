//
// image search, template maching
//
typedef struct __attribute__ ((packed)) paramStruct
{
    int smlHeight;
    int smlWidth;
    int bigRowPitch;
    int smlRowPitch;
    int pixelPitch;
    int sumArrayWidth;
    int sumArrayHeight;
} paramType;

__kernel void
search(__global const paramType *p,
       __global const unsigned char* inBigImg,
       __global const unsigned char* inSmlImg,
       __global int* sumArray)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int bigOffset=(y*p->bigRowPitch)+(x*p->pixelPitch);
    int smlOffset=0;

    int sum=0;
    for(int dy=0; dy<p->smlHeight; dy++)
    {
        for(int dx=0; dx<(p->smlWidth)*(p->pixelPitch); dx++)
        {
            sum+=abs(inBigImg[bigOffset+dx]
                        -inSmlImg[smlOffset+dx]);
        }
        bigOffset+=(p->bigRowPitch);
        smlOffset+=(p->smlRowPitch);
    }
    sumArray[y*p->sumArrayWidth+x]=sum;
}
