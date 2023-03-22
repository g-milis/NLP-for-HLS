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
float result_inner_0 [TILE_ROWS * GRID_COLS];
float temp_inner_0 [(TILE_ROWS + 2) * GRID_COLS];
float power_inner_0 [TILE_ROWS * GRID_COLS];
float result_inner_1 [TILE_ROWS * GRID_COLS];
float temp_inner_1 [(TILE_ROWS + 2) * GRID_COLS];
float power_inner_1 [TILE_ROWS * GRID_COLS];
float result_inner_2 [TILE_ROWS * GRID_COLS];
float temp_inner_2 [(TILE_ROWS + 2) * GRID_COLS];
float power_inner_2 [TILE_ROWS * GRID_COLS];
for (i = 0; i < SIM_TIME/2; i++) {
L24:        for (k = 0; k < GRID_ROWS / TILE_ROWS + 2; k++) {
            int load_flag = k >= 0 && k < GRID_ROWS / TILE_ROWS;
            int compute_flag = k >= 1 && k < GRID_ROWS / TILE_ROWS + 1;
            int store_flag = k >= 2 && k < GRID_ROWS / TILE_ROWS + 2;
            
            if (k % 3 == 0) {
                buffer_load_temp(load_flag, k, temp_inner_0, temp);
                buffer_load_power(load_flag, k, power_inner_0, power);

                buffer_compute(compute_flag, result_inner_2, temp_inner_2, power_inner_2, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, result, result_inner_1);
            } else if (k % 3 == 1) {
                buffer_load_temp(load_flag, k, temp_inner_1, temp);
                buffer_load_power(load_flag, k, power_inner_1, power);

                buffer_compute(compute_flag, result_inner_0, temp_inner_0, power_inner_0, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, result, result_inner_2);
            } else {
                buffer_load_temp(load_flag, k, temp_inner_2, temp);
                buffer_load_power(load_flag, k, power_inner_2, power);

                buffer_compute(compute_flag, result_inner_1, temp_inner_1, power_inner_1, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, result, result_inner_0);
            }
        }

L25:        for (k = 0; k < GRID_ROWS / TILE_ROWS + 2; k++) {
            int load_flag = k >= 0 && k < GRID_ROWS / TILE_ROWS;
            int compute_flag = k >= 1 && k < GRID_ROWS / TILE_ROWS + 1;
            int store_flag = k >= 2 && k < GRID_ROWS / TILE_ROWS + 2;
            
            if (k % 3 == 0) {
                                buffer_load_temp(load_flag, k, temp_inner_0, result);
                buffer_load_power(load_flag, k, power_inner_0, power);

                buffer_compute(compute_flag, result_inner_2, temp_inner_2, power_inner_2, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, temp, result_inner_1);
            } else if (k % 3 == 1) {
                buffer_load_temp(load_flag, k, temp_inner_1, result);
                buffer_load_power(load_flag, k, power_inner_1, power);

                buffer_compute(compute_flag, result_inner_0, temp_inner_0, power_inner_0, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, temp, result_inner_2);
            } else {
                buffer_load_temp(load_flag, k, temp_inner_2, result);
                buffer_load_power(load_flag, k, power_inner_2, power);

                buffer_compute(compute_flag, result_inner_1, temp_inner_1, power_inner_1, Cap_1, Rx_1, Ry_1, Rz_1, k - 1);
                buffer_store(store_flag, k - 2, temp, result_inner_0);
            }
        }
    }
