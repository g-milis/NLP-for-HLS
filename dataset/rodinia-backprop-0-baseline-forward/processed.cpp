float l1_buf[65537];
float conn_buf[TILE_SIZE*17];
float l2_buf[17];
for (j = 1; j < 17; j++) {
        
        sum = 0.0;

L5:        for (kk = 0; kk < 65536 + TILE_SIZE; kk += TILE_SIZE) {
            int size = (kk == 65536)?1:TILE_SIZE;
            memcpy(conn_buf, conn+kk*17, sizeof(float) * size * 17);
L6:            for (k = 0; k < TILE_SIZE; k++) {
                if (k + kk < 65537) {
                    float product = conn_buf[k * 17 + j] * l1_buf[k + kk];
                    sum += product;
                }
            }
        }
        
        l2_buf[j] = (1.0 / (1.0 + exp(-sum)));
    }
