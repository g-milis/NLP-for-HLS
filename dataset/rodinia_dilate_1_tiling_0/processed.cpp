bool strel[25] = { 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };
for (int i = 0; i < TILE_ROWS; i++) {
L3:	        for (int j = 0; j < GRID_COLS; j++) {

	            float max = 0.0, temp;

L4:	            for (int m = 0; m < STREL_ROWS; m++) {
L5:	                for (int n = 0; n < STREL_COLS; n++) {
	                    int p = i - radius_p + m;
	                    int q = j - radius_q + n;
	                    if ((p >= 0 || which_boundary != TOP) &&
	                    		q >= 0 &&
								(p < TILE_ROWS || which_boundary != BOTTOM) &&
								q < GRID_COLS &&
								strel[m * STREL_COLS + n] != 0)
	                    {
	                        temp = img[p * GRID_COLS + q + GRID_COLS * MAX_RADIUS];
	                        if (temp > max) max = temp;
	                    }
	                }
	            }
	            result[i * GRID_COLS + j] = max;
	        }
	    }
for (int row=0; row<TILE_ROWS+2*MAX_RADIUS; ++row){
L7:			for (int col=0; col<GRID_COLS; ++col){
				img_bram[row*GRID_COLS+col] = img[starting_index+row*GRID_COLS+col];
			}
		}
for (int row=0; row<TILE_ROWS; ++row){
L9:			for (int col=0; col<GRID_COLS; ++col){
				result[starting_index+row*GRID_COLS+col] = result_bram[row*GRID_COLS+col];
			}
		}
float result_inner_0 [TILE_ROWS * GRID_COLS];
float img_inner_0 [(TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS];
for (k = 0; k < GRID_ROWS / TILE_ROWS; k++) {
			load_data_tile(img_inner_0, img, k);
			lc_dilate(result_inner_0, img_inner_0, k);
			store_result_tile(result_inner_0, result, k);
		}
