__kernel void
clCode(__global int *a)
{
	int gi = get_global_id(0);
	int li = get_local_id(0);

        a[gi]  = li;
}
