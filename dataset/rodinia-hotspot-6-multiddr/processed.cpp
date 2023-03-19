float temp_top[PARA_FACTOR];
float temp_left[PARA_FACTOR];
float temp_right[PARA_FACTOR];
float temp_bottom[PARA_FACTOR];
float temp_center[PARA_FACTOR];
float power_center[PARA_FACTOR];
float temp_rf [PARA_FACTOR][GRID_COLS * 2 / PARA_FACTOR + 1];
for (i = 0 ; i < GRID_COLS * 2 / PARA_FACTOR + 1; i++) {
L9:        for (ii = 0; ii < PARA_FACTOR; ii++) {
            temp_rf[ii][i] = temp[i*PARA_FACTOR + ii];
        }
    }
for (i = 0; i < GRID_COLS / PARA_FACTOR * TILE_ROWS ; i++) {
    L11:        for (k = 0; k < PARA_FACTOR; k++) {
            temp_center[k]  = temp_rf[k][GRID_COLS / PARA_FACTOR];
            temp_top[k]     = (i < GRID_COLS / PARA_FACTOR && which_boundary == TOP) ? temp_center[k] : temp_rf[k][0];
            temp_left[k]    = ((i % (GRID_COLS / PARA_FACTOR)) == 0 && k == 0) ? 
                                temp_center[k] : temp_rf[(k - 1 + PARA_FACTOR) % PARA_FACTOR][GRID_COLS / PARA_FACTOR - (k == 0) ];
            temp_right[k]   = ((i % (GRID_COLS / PARA_FACTOR)) == (GRID_COLS / PARA_FACTOR - 1) && k == PARA_FACTOR - 1) ? 
                                temp_center[k] : temp_rf[(k + 1 + PARA_FACTOR) % PARA_FACTOR][GRID_COLS / PARA_FACTOR + (k == (PARA_FACTOR - 1)) ];
            temp_bottom[k]  = (i >= GRID_COLS / PARA_FACTOR * (TILE_ROWS - 1) && which_boundary == BOTTOM) ? 
                                temp_center[k] : temp_rf[k][GRID_COLS / PARA_FACTOR * 2];
            power_center[k] = power[i * PARA_FACTOR + k];
            result[i * PARA_FACTOR + k] = hotspot_stencil_core(temp_top[k], temp_left[k], temp_right[k], temp_bottom[k], temp_center[k], power_center[k], Cap_1, Rx_1, Ry_1, Rz_1);
        }

L12:        for (k = 0; k < PARA_FACTOR; k++) {
L13:            for (j = 0; j < GRID_COLS * 2 / PARA_FACTOR; j++) {
                temp_rf[k][j] = temp_rf[k][j + 1];
            }

            temp_rf[k][GRID_COLS * 2 / PARA_FACTOR] = temp[GRID_COLS * 2 + (i + 1) * PARA_FACTOR + k];
        }
    }
