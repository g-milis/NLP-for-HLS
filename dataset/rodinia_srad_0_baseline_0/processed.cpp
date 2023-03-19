float J_top[PARA_FACTOR];
float J_left[PARA_FACTOR];
float J_right[PARA_FACTOR];
float J_bottom[PARA_FACTOR];
float J_center[PARA_FACTOR];
float c_tmp[PARA_FACTOR];
float J_rf[PARA_FACTOR][COLS * 2 / PARA_FACTOR + 1];
float dN[(TILE_ROWS+1)*COLS];
float dS[(TILE_ROWS+1)*COLS];
float dW[(TILE_ROWS+1)*COLS];
float dE[(TILE_ROWS+1)*COLS];
float c[(TILE_ROWS+1)*COLS];
for (i = -2*COLS/PARA_FACTOR-1; i < COLS / PARA_FACTOR * (TILE_ROWS+1); i++) {
L14:    for (k = 0; k < PARA_FACTOR; k++) {
            J_center[k]  = J_rf[k][COLS / PARA_FACTOR];     
      J_top[k]     = (tile == TOP_TILE && i < COLS / PARA_FACTOR) ? J_center[k] : J_rf[k][0];
      J_left[k]    = ((i % (COLS / PARA_FACTOR)) == 0 && k == 0) ? J_center[k] : J_rf[(k - 1 + PARA_FACTOR) % PARA_FACTOR][COLS / PARA_FACTOR - (k == 0) ];
      J_right[k]   = ((i % (COLS / PARA_FACTOR)) == (COLS / PARA_FACTOR - 1) && k == PARA_FACTOR - 1) ? J_center[k] : J_rf[(k + 1 + PARA_FACTOR) % PARA_FACTOR][COLS / PARA_FACTOR + (k == (PARA_FACTOR - 1)) ];
      J_bottom[k]  = (tile == BOTTOM_TILE && i >= COLS / PARA_FACTOR * (TILE_ROWS - 1)) ? J_center[k] : J_rf[k][COLS / PARA_FACTOR * 2];

      if (i >= 0) {
			      	dN[i*PARA_FACTOR+k] = J_top[k] - J_center[k];
      	dS[i*PARA_FACTOR+k] = J_bottom[k] - J_center[k];
      	dW[i*PARA_FACTOR+k] = J_left[k] - J_center[k];
      	dE[i*PARA_FACTOR+k] = J_right[k] - J_center[k];

	      	c_tmp[k] = srad_core1(dN[i*PARA_FACTOR+k],
      			      dS[i*PARA_FACTOR+k],
      			      dW[i*PARA_FACTOR+k],
      			      dE[i*PARA_FACTOR+k],
      			      J_center[k], q0sqr);
                
	      	if (c_tmp[k] < 0) {
          c[i*PARA_FACTOR+k] = 0;
        }
      	else if (c_tmp[k] > 1) {
          c[i*PARA_FACTOR+k] = 1;
        }
      	else {
          c[i*PARA_FACTOR+k] = c_tmp[k];
        }
	      }
    }

    L15:    for (k = 0; k < PARA_FACTOR; k++) {
L16:      for (j = 0; j < COLS * 2 / PARA_FACTOR; j++) {
        J_rf[k][j] = J_rf[k][j + 1];
      }

      J_rf[k][COLS * 2 / PARA_FACTOR] = J[2*COLS + (i + 1) * PARA_FACTOR + k];
    }
  }
