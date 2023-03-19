for(t = 0; t < TILE_SIZE; t+=2){
L2:		KERNEL_COMP_1: for(n = 0; n < COLS/2; n++){

			int32_t get1, get2;
			int32_t min1, min2;
			int32_t oldMin1, oldMin2, oldMin3;
			int32_t next;
		
			get1 = (int)J_buf[t * COLS + n*2];	
			get2 = (int)J_buf[t * COLS + n*2+1];

			oldMin1 = min1 = Jout_buf1[n];
			oldMin2 = min2 = Jout_buf3[n];
			next = Jout_buf1[n+1];
		

			if(n > 0){
				min1 = MIN(min1,extra);
			}

			min1 = MIN(min1,min2);
		
			extra = min2;
					
			min2 = MIN(min2,oldMin1);

			if(n < COLS/2){
				min2 = MIN(min2,next);
			}	

			Jout_buf2[n] = get1+min1;
			Jout_buf4[n] = get2+min2;
		}

L3:		KERNEL_COMP_2: for(n = 0; n < COLS/2; n++){

			int32_t get1, get2;
			int32_t min1, min2;
			int32_t oldMin1, oldMin2, oldMin3;
			int32_t next;
		
			get1 = (int)J_buf[(t+1) * COLS + n*2];	
			get2 = (int)J_buf[(t+1) * COLS + n*2+1];

			oldMin1 = min1 = Jout_buf2[n];
			oldMin2 = min2 = Jout_buf4[n];
			next = Jout_buf2[n+1];
	
			if(n > 0){
				min1 = MIN(min1,extra);
			}

			min1 = MIN(min1,min2);
		
			extra = min2;
					
			min2 = MIN(min2,oldMin1);

			if(n < COLS/2){
				min2 = MIN(min2,next);
			}	
		
			
			Jout_buf1[n] = get1+min1;
			Jout_buf3[n] = get2+min2;
		}
	}
char J_buf1[COLS*TILE_SIZE];
char J_buf2[COLS*TILE_SIZE];
int32_t Jout_buf1[COLS/2];
int32_t Jout_buf2[COLS/2];
int32_t Jout_buf3[COLS/2];
int32_t Jout_buf4[COLS/2];
int32_t Jtemp[COLS*TILE_SIZE/4];
for(t = 0; t < COLS/4; t++){
		Jout_buf3[t * 2 + 1] = (Jtemp[t] >> 24) & 0xFF;
		Jout_buf1[t * 2 + 1] = (Jtemp[t] >> 16) & 0xFF;
		Jout_buf3[t * 2] = (Jtemp[t] >> 8) & 0xFF;
		Jout_buf1[t * 2] = Jtemp[t] & 0xFF;
	}
for(t = 1; t < ROWS/TILE_SIZE;t++){
		if(t % 2 == 0){
			load(J_buf2,J,t);
			pathfinder_kernel(J_buf1,Jout_buf1,Jout_buf2,Jout_buf3,Jout_buf4);
		}else if(t % 2 == 1){
			load(J_buf1,J,t);
			pathfinder_kernel(J_buf2,Jout_buf1,Jout_buf2,Jout_buf3,Jout_buf4);
		}	
	}
for(t = 0; t < COLS/2; t++){
		Jtemp[t*2] = Jout_buf2[t];
		Jtemp[t*2+1] = Jout_buf4[t];
	}
