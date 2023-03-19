for (int r = 0; r < TILE_ROWS; r++)
L2:        for (int c = 0; c < GRID_COLS; c++) {
    
            float center = temp[GRID_COLS + r*GRID_COLS + c];
            float top = ((which_boundary == TOP) && r == 0) ? center : temp[GRID_COLS + (r - 1)*GRID_COLS + c];
            float right = (c == GRID_COLS - 1) ? center : temp[GRID_COLS + r*GRID_COLS + c + 1];
            float bottom = ((which_boundary == BOTTOM) && r == TILE_ROWS - 1) ? center : temp[GRID_COLS + (r + 1)*GRID_COLS + c];
            float left = (c == 0) ? center : temp[GRID_COLS + r*GRID_COLS + c - 1];

            delta = (Cap_1 * (power[r*GRID_COLS + c] +
                    (top + bottom - 2.f*center) * Ry_1 +
                    (right + left - 2.f*center) * Rx_1 +
                    (amb_temp - center) * Rz_1));
                    
            result[r*GRID_COLS + c] = temp[GRID_COLS + r*GRID_COLS + c] + delta;
        }
float result_inner [TILE_ROWS * GRID_COLS];
float temp_inner [(TILE_ROWS + 2) * GRID_COLS];
float power_inner [TILE_ROWS * GRID_COLS];
for (i = 0; i < SIM_TIME/2; i++) {
        int k;
L7:        for (k = 0; k < GRID_ROWS / TILE_ROWS; k++) {                                                                                    
            memcpy(temp_inner, temp + k * TILE_ROWS * GRID_COLS - GRID_COLS, sizeof(float) * (TILE_ROWS + 2) * GRID_COLS);
            memcpy(power_inner, power + k * TILE_ROWS * GRID_COLS, sizeof(float) * TILE_ROWS * GRID_COLS);
             
            hotspot(result_inner, temp_inner, power_inner, Cap_1, Rx_1, Ry_1, Rz_1, k);
                          
            memcpy(result + k * TILE_ROWS * GRID_COLS, result_inner, sizeof(float) * TILE_ROWS * GRID_COLS);
        }
 
L8:        for (k = 0; k < GRID_ROWS / TILE_ROWS; k++) {
            memcpy(temp_inner, result + k * TILE_ROWS * GRID_COLS - GRID_COLS, sizeof(float) * (TILE_ROWS + 2) * GRID_COLS);
            memcpy(power_inner, power + k * TILE_ROWS * GRID_COLS, sizeof(float) * TILE_ROWS * GRID_COLS);
 
            hotspot(result_inner, temp_inner, power_inner, Cap_1, Rx_1, Ry_1, Rz_1, k);
 
            memcpy(temp + k * TILE_ROWS * GRID_COLS, result_inner,  sizeof(float) * TILE_ROWS * GRID_COLS);
        }
    }
