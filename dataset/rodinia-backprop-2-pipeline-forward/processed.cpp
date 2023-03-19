float sum[16];
float l1_buf[65536];
float conn_buf[TILE_SIZE][16];
float l2_buf[16];
for (kk = 1; kk < 65537; kk += TILE_SIZE) {
        memcpy(conn_buf[0], conn+kk*16, sizeof(float) * TILE_SIZE * 16);
L6:        LOOP2: for (k = 0; k < TILE_SIZE; k++) {
L7:            LOOP3: for (j = 0; j < 16; j++) {
                float product = conn_buf[k][j] * l1_buf[k + kk - 1];
                sum[j] += product;
            }
        }
    }
for (j = 0; j < 16; j++) {
        l2_buf[j] = (1.0 / (1.0 + exp(-sum[j])));
    }
