for (int i = 0; i < TILE_ROWS; i++) {
L2:        for (int j = 0; j < GRID_COLS; j++) {
            float old_val = imgvf[i * GRID_COLS + j + GRID_COLS];

            float UL    = ((i == 0 && which_boundary == TOP              ||  j == 0              ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j - 1  ) + GRID_COLS] - old_val));
            float U     = ((i == 0 && which_boundary == TOP                                      ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j      ) + GRID_COLS] - old_val));
            float UR    = ((i == 0 && which_boundary == TOP              ||  j == GRID_COLS - 1  ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j + 1  ) + GRID_COLS] - old_val));

            float L     = ((                        j == 0              ) ? 0 : (imgvf[(i       ) * GRID_COLS + (j - 1  ) + GRID_COLS] - old_val));
            float R     = ((                        j == GRID_COLS - 1  ) ? 0 : (imgvf[(i       ) * GRID_COLS + (j + 1  ) + GRID_COLS] - old_val));

            float DL    = (((i == TILE_ROWS - 1)  && which_boundary == BOTTOM ||  j == 0              ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j - 1  ) + GRID_COLS] - old_val));
            float D     = (((i == TILE_ROWS - 1)  && which_boundary == BOTTOM                         ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j      ) + GRID_COLS] - old_val));
            float DR    = (((i == TILE_ROWS - 1)  && which_boundary == BOTTOM ||  j == GRID_COLS - 1  ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j + 1  ) + GRID_COLS] - old_val));

            float vHe = old_val + MU_O_LAMBDA * (heaviside(UL) * UL + heaviside(U) * U + heaviside(UR) * UR + heaviside(L) * L + heaviside(R) * R + heaviside(DL) * DL + heaviside(D) * D + heaviside(DR) * DR);

            float vI = I[i * GRID_COLS + j];
            float new_val = vHe - (ONE_O_LAMBDA * vI * (vHe - vI));
            result[i * GRID_COLS + j] = new_val;

        }
    }
float result_inner [TILE_ROWS * GRID_COLS];
float imgvf_inner [(TILE_ROWS + 2) * GRID_COLS];
float I_inner [TILE_ROWS * GRID_COLS];
for (i = 0; i < ITERATION / 2; i++) {
    	int k;
L7:    	for(k = 0; k < GRID_ROWS / TILE_ROWS; k++) {

    		memcpy(imgvf_inner, imgvf + k * TILE_ROWS * GRID_COLS - GRID_COLS, sizeof(float) * (TILE_ROWS + 2) * GRID_COLS);
    		memcpy(I_inner, I + k * TILE_ROWS * GRID_COLS, sizeof(float) * TILE_ROWS * GRID_COLS);

        		lc_mgvf(result_inner, imgvf_inner, I_inner, k);

    		memcpy(result + k * TILE_ROWS * GRID_COLS, result_inner, sizeof(float) * TILE_ROWS * GRID_COLS);

    	}

L8:    	for(k = 0; k < GRID_ROWS / TILE_ROWS; k++) {

    		memcpy(imgvf_inner, result + k * TILE_ROWS * GRID_COLS - GRID_COLS, sizeof(float) * (TILE_ROWS + 2) * GRID_COLS);
    		memcpy(I_inner, I + k * TILE_ROWS * GRID_COLS, sizeof(float) * TILE_ROWS * GRID_COLS);

        		lc_mgvf(result_inner, imgvf_inner, I_inner, k);

    		memcpy(imgvf + k * TILE_ROWS * GRID_COLS, result_inner, sizeof(float) * TILE_ROWS * GRID_COLS);

    	}

	}
