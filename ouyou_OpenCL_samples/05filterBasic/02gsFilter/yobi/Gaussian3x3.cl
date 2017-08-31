/*
 * Gaussian 3x3
 *
 *      | 1 2 1 |
 * 1/16 | 2 4 2 |
 *      | 1 2 1 |
 *
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

        +in[row0+x+0]   +in[row0+x+1]*2  +in[row0+x+2]

        +in[row1+x+0]*2 +in[row1+x+1]*4  +in[row1+x+2]*2

        +in[row2+x+0]   +in[row2+x+1]*2  +in[row2+x+2] );

    out[row1+x+1]=intVal/16;
}
