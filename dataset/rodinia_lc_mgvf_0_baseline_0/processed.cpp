for (int i = 0; i < GRID_ROWS; i++) {
L2:        for (int j = 0; j < GRID_COLS; j++) {
            float old_val = imgvf[i * GRID_COLS + j];

            float UL    = ((i == 0              ||  j == 0              ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j - 1  )] - old_val));
            float U     = ((i == 0                                      ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j      )] - old_val));
            float UR    = ((i == 0              ||  j == GRID_COLS - 1  ) ? 0 : (imgvf[(i - 1   ) * GRID_COLS + (j + 1  )] - old_val));

            float L     = ((                        j == 0              ) ? 0 : (imgvf[(i       ) * GRID_COLS + (j - 1  )] - old_val));
            float R     = ((                        j == GRID_COLS - 1  ) ? 0 : (imgvf[(i       ) * GRID_COLS + (j + 1  )] - old_val));

            float DL    = ((i == GRID_ROWS - 1  ||  j == 0              ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j - 1  )] - old_val));
            float D     = ((i == GRID_ROWS - 1                          ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j      )] - old_val));
            float DR    = ((i == GRID_ROWS - 1  ||  j == GRID_COLS - 1  ) ? 0 : (imgvf[(i + 1   ) * GRID_COLS + (j + 1  )] - old_val));

            float vHe = old_val + MU_O_LAMBDA * (heaviside(UL) * UL + heaviside(U) * U + heaviside(UR) * UR + heaviside(L) * L + heaviside(R) * R + heaviside(DL) * DL + heaviside(D) * D + heaviside(DR) * DR);

            float vI = I[i * GRID_COLS + j];
            float new_val = vHe - (ONE_O_LAMBDA * vI * (vHe - vI));
            result[i * GRID_COLS + j] = new_val;

            total_diff += fabs(new_val - old_val);
        }
    }
for (i = 0; i < ITERATION / 2; i++) {
        diff = lc_mgvf(result, imgvf, I);
        diff = lc_mgvf(imgvf, result, I);
    }
