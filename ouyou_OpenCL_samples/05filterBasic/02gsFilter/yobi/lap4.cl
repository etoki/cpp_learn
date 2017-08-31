/*
 * lap4, input=mono
 */
__kernel void
clCode(const unsigned int width,
       __global const unsigned char* in,
       __global unsigned char* out)
{
    size_t x=get_global_id(0);
    size_t y=get_global_id(1);

    int row0=width*(y+0);
    int row1=width*(y+1);
    int row2=width*(y+2);

    int intVal=(int)(
                        -in[row0+x+1]

        -in[row1+x+0]  +in[row1+x+1]*4  -in[row1+x+2]

                      -in[row2+x+1] );

    out[row1+x+1]=convert_uchar_sat(intVal);
}
