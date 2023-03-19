float l1_buf[65536];
float conn_buf[TILE_SIZE*16];
float l2_buf[16];
float conn0_buf[16];
for (j = 0; j < 16; j++) {

        
        sum = conn0_buf[j];

L6:        for (kk = 1; kk < 65537; kk += TILE_SIZE) {
            memcpy(conn_buf, conn+kk*16, sizeof(float) * TILE_SIZE * 16);
L7:            for (k = 0; k < TILE_SIZE; k++) {
                float product = conn_buf[k * 16 + j] * l1_buf[k+kk-1];
                sum += product;
            }
        }

        l2_buf[j] = (1.0 / (1.0 + exp(-sum)));
    }
