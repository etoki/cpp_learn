__kernel void
clCode(__global int *a)
{
	int globalId = get_global_id(0);
	int numOfGroups = get_num_groups(0);
	int groupId = get_group_id(0);
	int localId = get_local_id(0);

	a[globalId * 4 + 0] = globalId;
	a[globalId * 4 + 1] = numOfGroups;
	a[globalId * 4 + 2] = groupId;
	a[globalId * 4 + 3] = localId;

}
