for (int r = 0; r < GRID_ROWS; r++)
L2:        for (int c = 0; c < GRID_COLS; c++) {
            if (r == 0 || c == 0 || r == GRID_ROWS - 1 || c == GRID_COLS - 1) {

                
                if ((r == 0) && (c == 0)) {
                    delta = (Cap_1) * (power[0] +
                        (temp[1] - temp[0]) * Rx_1 +
                        (temp[GRID_COLS] - temp[0]) * Ry_1 +
                        (amb_temp - temp[0]) * Rz_1);
                }   
    
                
                else if ((r == 0) && (c == GRID_COLS - 1)) {
                    delta = (Cap_1) * (power[c] +
                        (temp[c - 1] - temp[c]) * Rx_1 +
                        (temp[c + GRID_COLS] - temp[c]) * Ry_1 +
                        (amb_temp - temp[c]) * Rz_1);
                }   
    
                
                else if ((r == GRID_ROWS - 1) && (c == GRID_COLS - 1)) {
                    delta = (Cap_1) * (power[r*GRID_COLS + c] +
                        (temp[r*GRID_COLS + c - 1] - temp[r*GRID_COLS + c]) * Rx_1 +
                        (temp[(r - 1)*GRID_COLS + c] - temp[r*GRID_COLS + c]) * Ry_1 +
                        (amb_temp - temp[r*GRID_COLS + c]) * Rz_1);
                }   
    
                
                else if ((r == GRID_ROWS - 1) && (c == 0)) {
                    delta = (Cap_1) * (power[r*GRID_COLS] +
                        (temp[r*GRID_COLS + 1] - temp[r*GRID_COLS]) * Rx_1 +
                        (temp[(r - 1)*GRID_COLS] - temp[r*GRID_COLS]) * Ry_1 +
                        (amb_temp - temp[r*GRID_COLS]) * Rz_1);
                }   
    
                
                else if (r == 0) {
                    delta = (Cap_1) * (power[c] +
                        (temp[c + 1] + temp[c - 1] - 2.0*temp[c]) * Rx_1 +
                        (temp[GRID_COLS + c] - temp[c]) * Ry_1 +
                        (amb_temp - temp[c]) * Rz_1);
                }   
    
                
                else if (c == GRID_COLS - 1) {
                    delta = (Cap_1) * (power[r*GRID_COLS + c] +
                        (temp[(r + 1)*GRID_COLS + c] + temp[(r - 1)*GRID_COLS + c] - 2.0*temp[r*GRID_COLS + c]) * Ry_1 +
                        (temp[r*GRID_COLS + c - 1] - temp[r*GRID_COLS + c]) * Rx_1 +
                        (amb_temp - temp[r*GRID_COLS + c]) * Rz_1);
                }   
    
                
                else if (r == GRID_ROWS - 1) {
                    delta = (Cap_1) * (power[r*GRID_COLS + c] +
                        (temp[r*GRID_COLS + c + 1] + temp[r*GRID_COLS + c - 1] - 2.0*temp[r*GRID_COLS + c]) * Rx_1 +
                        (temp[(r - 1)*GRID_COLS + c] - temp[r*GRID_COLS + c]) * Ry_1 +
                        (amb_temp - temp[r*GRID_COLS + c]) * Rz_1);
                }   
    
                
                else if (c == 0) {
                    delta = (Cap_1) * (power[r*GRID_COLS] +
                        (temp[(r + 1)*GRID_COLS] + temp[(r - 1)*GRID_COLS] - 2.0*temp[r*GRID_COLS]) * Ry_1 +
                        (temp[r*GRID_COLS + 1] - temp[r*GRID_COLS]) * Rx_1 +
                        (amb_temp - temp[r*GRID_COLS]) * Rz_1);
                }

            }

            else {
                    delta = (Cap_1 * (power[r*GRID_COLS + c] +
                        (temp[(r + 1)*GRID_COLS + c] + temp[(r - 1)*GRID_COLS + c] - 2.f*temp[r*GRID_COLS + c]) * Ry_1 +
                        (temp[r*GRID_COLS + c + 1] + temp[r*GRID_COLS + c - 1] - 2.f*temp[r*GRID_COLS + c]) * Rx_1 +
                        (amb_temp - temp[r*GRID_COLS + c]) * Rz_1));
            }

            result[r*GRID_COLS + c] = temp[r*GRID_COLS + c] + delta;

        }
for (i = 0; i < SIM_TIME/2; i++) {
       hotspot(result, temp, power, Cap_1, Rx_1, Ry_1, Rz_1);
       
       hotspot(temp, result, power, Cap_1, Rx_1, Ry_1, Rz_1);

    }
