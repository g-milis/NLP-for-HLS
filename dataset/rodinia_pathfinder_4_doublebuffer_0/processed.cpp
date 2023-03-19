for(tt = 0; tt < TILE_SIZE; tt++){
L2:		KERNEL_INNER: for(n = 0; n < COLS; n++){
			min = dst[n];
			
			if(n > 0){
				min = MIN(min,dst[n-1]);
			}
	
			if(n < COLS-1){
				min = MIN(min,dst[n+1]);
			}

			if(t * TILE_SIZE + tt < ROWS-1){
				src[n] = wall[tt * COLS + n]+min;	
			}
		}

L3:		KERNEL_SWAP:for(n = 0; n < COLS/2; n++){
			dst[2*n] = src[2*n];
			dst[2*n+1] = src[2*n+1];
		}
	}
int32_t dst[COLS];
int32_t src[COLS];
int32_t wall[COLS * TILE_SIZE];
int32_t wall2[COLS * TILE_SIZE];
for(t = 1; t < ROWS/TILE_SIZE ;t++){
		if(t % 2 == 0){
			load(wall,J,t);
			pathfinder_kernel(dst,src,wall2,t-1);
		}else if(t % 2 == 1){
			load(wall2,J,t);
			pathfinder_kernel(dst,src,wall,t-1);
		}
	}
