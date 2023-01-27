
void local_sum(__local float* data)
{
	const int local_x = get_local_id(0);
	const int local_width = get_local_size(0);
	
	for(int offset = local_width / 2; offset > 0; offset /= 2) {
		if(local_x < offset) {
			data[local_x] += data[local_x + offset];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
}

