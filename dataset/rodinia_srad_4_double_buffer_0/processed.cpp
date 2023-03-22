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
          c[i*PARA_FACTOR+k] = 0;}
        else if (c_tmp[k] > 1) {
          c[i*PARA_FACTOR+k] = 1;}
        else {
          c[i*PARA_FACTOR+k] = c_tmp[k];}
                    }
    }

    L15:    for (k = 0; k < PARA_FACTOR; k++) {
L16:      for (j = 0; j < COLS * 2 / PARA_FACTOR; j++) {
        J_rf[k][j] = J_rf[k][j + 1];
      }
      J_rf[k][COLS * 2 / PARA_FACTOR] = J[2*COLS + (i + 1) * PARA_FACTOR + k];
    }
  }
float c_right[PARA_FACTOR];
float c_bottom[PARA_FACTOR];
float c_center[PARA_FACTOR];
float c_rf[PARA_FACTOR][COLS / PARA_FACTOR + 1];
for (i = -COLS/PARA_FACTOR-1; i < COLS / PARA_FACTOR * TILE_ROWS; i++) {
L22:    for (k = 0; k < PARA_FACTOR; k++) {
            c_center[k]  = c_rf[k][0];

      c_right[k]   = ((i % (COLS / PARA_FACTOR)) == (COLS / PARA_FACTOR - 1) && k == PARA_FACTOR - 1) ? c_center[k] : c_rf[(k + 1 + PARA_FACTOR) % PARA_FACTOR][ (k == (PARA_FACTOR - 1)) ];

      c_bottom[k]  = (tile == BOTTOM_TILE && i >= COLS / PARA_FACTOR * (TILE_ROWS - 1)) ? c_center[k] : c_rf[k][COLS / PARA_FACTOR];

      if (i >= 0) {
      	Jout[i*PARA_FACTOR+k] = srad_core2(dN[i*PARA_FACTOR+k], dS[i*PARA_FACTOR+k],
      					   dW[i*PARA_FACTOR+k], dE[i*PARA_FACTOR+k],
      					   c_center[k], c_bottom[k], c_center[k], c_right[k],
      					   J[COLS+i*PARA_FACTOR+k]);
      }
    }

    L23:    for (k = 0; k < PARA_FACTOR; k++) {
L24:      for (j = 0; j < COLS / PARA_FACTOR; j++) {
        c_rf[k][j] = c_rf[k][j + 1];
      }
      c_rf[k][COLS / PARA_FACTOR] = c[COLS + (i + 1) * PARA_FACTOR + k];
    }
  }
float J_buf_0[(TILE_ROWS+3)*COLS];
float Jout_buf_0[TILE_ROWS*COLS];
float J_buf_1[(TILE_ROWS+3)*COLS];
float Jout_buf_1[TILE_ROWS*COLS];
float J_buf_2[(TILE_ROWS+3)*COLS];
float Jout_buf_2[TILE_ROWS*COLS];
for (iter=0; iter<NITER/2; iter++){
                  L32:    TILE_LOOP1: for (t = 0; t < ROWS/TILE_ROWS + 2; t++) {
    	int load_flag = t >= 0 && t < ROWS / TILE_ROWS;
    	int compute_flag = t >= 1 && t < ROWS / TILE_ROWS + 1;
    	int store_flag = t >= 2 && t < ROWS / TILE_ROWS + 2;

    	if (t % 3 == 0) {
    	  buffer_load(load_flag, J_buf_0, J, t);
    	      	  buffer_compute(compute_flag, J_buf_2, Jout_buf_2, v0sqr, t-1);
    	  buffer_store(store_flag, Jout, Jout_buf_1, t-2);
    	} else if (t % 3 == 1) {
    	  buffer_load(load_flag, J_buf_1, J, t);
    	  buffer_compute(compute_flag, J_buf_0, Jout_buf_0, v0sqr, t-1);
    	  buffer_store(store_flag, Jout, Jout_buf_2, t-2);
    	} else if (t % 3 == 2) {
    	  buffer_load(load_flag, J_buf_2, J, t);
    	  buffer_compute(compute_flag, J_buf_1, Jout_buf_1, v0sqr, t-1);
    	  buffer_store(store_flag, Jout, Jout_buf_0, t-2);
    	}
    }

      L33:    TILE_LOOP2: for (t = 0; t < ROWS/TILE_ROWS + 2; t++) {
    	int load_flag = t >= 0 && t < ROWS / TILE_ROWS;
    	int compute_flag = t >= 1 && t < ROWS / TILE_ROWS + 1;
    	int store_flag = t >= 2 && t < ROWS / TILE_ROWS + 2;

    	if (t % 3 == 0) {
    	  buffer_load(load_flag, J_buf_0, Jout, t);
    	  buffer_compute(compute_flag, J_buf_2, Jout_buf_2, v0sqr, t-1);
    	  buffer_store(store_flag, J, Jout_buf_1, t-2);
    	} else if (t % 3 == 1) {
    	  buffer_load(load_flag, J_buf_1, Jout, t);
    	  buffer_compute(compute_flag, J_buf_0, Jout_buf_0, v0sqr, t-1);
    	  buffer_store(store_flag, J, Jout_buf_2, t-2);
    	} else if (t % 3 == 2) {
    	  buffer_load(load_flag, J_buf_2, Jout, t);
    	  buffer_compute(compute_flag, J_buf_1, Jout_buf_1, v0sqr, t-1);
    	  buffer_store(store_flag, J, Jout_buf_0, t-2);
    	}
    }

  }
