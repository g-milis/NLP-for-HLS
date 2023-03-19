int32_t dst[COLS];
int32_t src[COLS];
int32_t wall[COLS * TILE_SIZE];
for(t = 0; t < ROWS/TILE_SIZE ;t++){
		memcpy(wall,J + COLS*(t*TILE_SIZE+1),sizeof(int32_t) * COLS * TILE_SIZE);

L5:		KERNEL_TILE:for(tt = 0; tt < TILE_SIZE; tt++){
L6:			KERNEL_INNER: for(n = 0; n < COLS; n++){
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
			memcpy(dst,src,sizeof(int32_t) * COLS);
		}
	}
