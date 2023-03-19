for (int i = 0; i < STREL_ROWS; i++)
L2:	        for (int j = 0; j < STREL_COLS; j++) {
	            float temp = img_sample[i * STREL_COLS + j];
	            if (temp > max) max = temp;
	        }
bool strel[25] = { 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };
float img_rf[(TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS * 2 + PARA_FACTOR];
for (int i = 0; i < MAX_RADIUS; i++) {
	        img_rf[i] = 0.0;
	    }
for (int i = 0; i < (TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS + PARA_FACTOR; i++) {
	        img_rf[i + MAX_RADIUS] = img[i];
	    }
for (int i = 0; i < ((TILE_COLS+2*MAX_RADIUS) * TILE_ROWS) / PARA_FACTOR ; i++) {

L8:	        UNROLL_PE : for (int k = 0; k < PARA_FACTOR; k++) {
L9:	            float img_sample[STREL_ROWS * STREL_COLS];

L10:	            FILTER_ROW : for (int m = 0; m < STREL_ROWS; m++) {
L11:	            	FILTER_COL : for (int n = 0; n < STREL_COLS; n++) {
						if ( strel[m * STREL_COLS + n] != 1 )
	                    {
	                    	img_sample[m * STREL_COLS + n] = 0;
	                    }
	                    else {
	                    	img_sample[m * STREL_COLS + n] = img_rf[(TILE_COLS+2*MAX_RADIUS) * m + n + k];
	                    }
	                }
	            }
	            result[i * PARA_FACTOR + k] = lc_dilate_stencil_core(img_sample);
	        }

L12:	        SHIFT_AHEAD_BODY_INDEX : for (int k = 0; k < (TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS * 2; k++) {
	            img_rf[k] = img_rf[k + PARA_FACTOR];
	        }

L13:	        SHIFT_AHEAD_LAST_INDEX : for (int k = 0; k < PARA_FACTOR; k++) {
	        	if ((TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS + (i + 1) * PARA_FACTOR + k <
	        		(TILE_ROWS + 2 * MAX_RADIUS) * (TILE_COLS + 2 * MAX_RADIUS)){
					img_rf[(TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS * 2 + k] =
							img[(TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS + (i + 1) * PARA_FACTOR + k];
	        	}else{
	        		img_rf[(TILE_COLS+2*MAX_RADIUS) * (2 * MAX_RADIUS) + MAX_RADIUS * 2 + k] = 0.0;
	        	}
	        }
	    }
for (int row=0; row<TILE_ROWS+2*MAX_RADIUS; ++row){
L15:			for (int col=0; col<GRID_COLS; ++col){
				img_bram[row*GRID_COLS+col] = img[starting_index+row*GRID_COLS+col];
			}
		}
for (int row=0; row<TILE_ROWS; ++row){
L17:			for (int col=0; col<GRID_COLS; ++col){
				result[starting_index+row*GRID_COLS+col] = result_bram[row*GRID_COLS+col];
			}
		}
float result_inner_0 [TILE_ROWS * GRID_COLS];
float img_inner_0 [(TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS];
float result_inner_col [TILE_ROWS * (TILE_COLS+2*MAX_RADIUS)];
float img_inner_col [(TILE_ROWS + 2 * MAX_RADIUS) * (TILE_COLS + 2 * MAX_RADIUS)];
for (int k = 0; k < GRID_ROWS / TILE_ROWS; k++) {
			load_data_tile(img_inner_0, img, k);

L23:			COL_TILES : for (int j = 0; j < GRID_COLS / TILE_COLS; ++j){

				if (j == 0){
					start_col = 0;
L24:					LOAD_IMG_ROW_LEFTMOST : for (int row=0; row<(TILE_ROWS + 2 * MAX_RADIUS); ++row){
L25:						LOAD_IMG_COL_LEFTMOST : for (int col=0; col<(TILE_COLS + MAX_RADIUS); ++col){
							img_inner_col[row*(TILE_COLS + 2 * MAX_RADIUS)+MAX_RADIUS+col] = img_inner_0[row*GRID_COLS+col];
						}
					}
L26:					LOAD_IMG_ROW_LEFTMOST_BLANK : for (int row=0; row<(TILE_ROWS + 2 * MAX_RADIUS); ++row){
L27:						LOAD_IMG_COL_LEFTMOST_BLANK : for (int col=0; col<MAX_RADIUS; ++col){
							img_inner_col[row*(TILE_COLS + 2 * MAX_RADIUS)+col] = 0.0;
						}
					}
				}
				else if (j == GRID_COLS / TILE_COLS-1){
					start_col = j * TILE_COLS - MAX_RADIUS;
L28:					LOAD_IMG_ROW_RIGHTMOST : for (int row=0; row<(TILE_ROWS + 2 * MAX_RADIUS); ++row){
L29:						LOAD_IMG_COL_RIGHTMOST : for (int col=0; col<(TILE_COLS + MAX_RADIUS); ++col){
							img_inner_col[row*(TILE_COLS + 2 * MAX_RADIUS)+col] = img_inner_0[row*GRID_COLS+start_col+col];
						}
					}
L30:					LOAD_IMG_ROW_RIGHTMOST_BLANK : for (int row=0; row<(TILE_ROWS + 2 * MAX_RADIUS); ++row){
L31:						LOAD_IMG_COL_RIGHTMOST_BLANK : for (int col=0; col<MAX_RADIUS; ++col){
							img_inner_col[row*(TILE_COLS + 2 * MAX_RADIUS)+(TILE_COLS + MAX_RADIUS)+col] = 0.0;
						}
					}
				}
				else{
					start_col = j * TILE_COLS - MAX_RADIUS;
L32:					LOAD_IMG_ROW_REST : for (int row=0; row<(TILE_ROWS + 2 * MAX_RADIUS); ++row){
L33:						LOAD_IMG_COL_REST : for (int col=0; col<(TILE_COLS + 2 * MAX_RADIUS); ++col){
							img_inner_col[row*(TILE_COLS + 2 * MAX_RADIUS)+col] = img_inner_0[row*GRID_COLS+start_col+col];
						}
					}
				}

				lc_dilate(result_inner_col, img_inner_col, k);

				start_col = j * TILE_COLS;
L34:				STORE_RST_ROW : for (int row=0; row<TILE_ROWS; ++row){
L35:					STORE_RST_COL : for (int col=0; col<TILE_COLS; ++col){
						result_inner_0[row*GRID_COLS+start_col+col] = result_inner_col[row*(TILE_COLS+2*MAX_RADIUS)+MAX_RADIUS+col];
					}
				}
			}

			store_result_tile(result_inner_0, result, k);
		}
