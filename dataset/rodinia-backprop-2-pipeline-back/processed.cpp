float w0_buf[16];
float oldw0_buf[16];
float delta_buf[16];
float ly_buf[65536];
float w_buf[TILE_SIZE][16];
float oldw_buf[TILE_SIZE][16];
for (kk = 1; kk < 65537; kk+=TILE_SIZE) {
        memcpy(w_buf[0], w+kk*16, sizeof(float) * TILE_SIZE * 16);
        memcpy(oldw_buf[0], oldw+kk*16, sizeof(float) * TILE_SIZE * 16);
L8:        LOOP2:for (k = 0; k < TILE_SIZE; k++) {
L9:            LOOP3: for (j = 0; j < 16; j++) {
                new_dw = ETA * delta_buf[j] * ly_buf[k+kk-1] + MOMENTUM * oldw_buf[k][j];
                w_buf[k][j] += new_dw;
                oldw_buf[k][j] = new_dw;
            }
        }
        memcpy(w+kk*16, w_buf[0], sizeof(float) * TILE_SIZE * 16);
        memcpy(oldw+kk*16, oldw_buf[0], sizeof(float) * TILE_SIZE * 16);
    }
for (j = 0; j < 16; j++) {
        new_dw = ETA * delta_buf[j] + MOMENTUM * oldw0_buf[j];
        w0_buf[j] += new_dw;
        oldw0_buf[j] = new_dw;
    }
