for (k = 0; k < TILE_SIZE; k+=UNROLL_FACTOR) {
L2:            for(i = 0; i < UNROLL_FACTOR; i++) {
L3:                LOOP3: for (j = 0; j < 16; j++) {
                    float product = conn_buf[k+i][j] * l1_buf[k+i + kk];
                    sum[i][j] += product;
                }
            }
        }
float sum[UNROLL_FACTOR][16];
float l1_buf[65536];
float conn_buf_x[TILE_SIZE][16];
float conn_buf_y[TILE_SIZE][16];
float l2_buf[16];
for(int loop = 0; loop < 2000; ++loop) {
            memcpy_wide_bus_read_float(l1_buf, l1, sizeof(float), sizeof(float) * 65536);

            memcpy_wide_bus_read_float(sum[0], conn, 0, sizeof(float) * 16);

L10:            for (i = 1; i < UNROLL_FACTOR; i++) {
L11:                for (j = 0; j < 16; j++) {
                    sum[i][j] = 0.0;
                }
            }

            LOOP1:
L12:            for (kk = 0; kk < 65536 + TILE_SIZE; kk += TILE_SIZE) {
                int load_flag = (kk < 65536);
                int compute_flag = (kk >= TILE_SIZE);
                i = (kk / TILE_SIZE) % 2;

                if (i == 0) {
                    buffer_load(load_flag, kk, conn, conn_buf_x);
                    buffer_compute(compute_flag, kk, l1_buf, conn_buf_y, sum);
                } else {
                    buffer_load(load_flag, kk, conn, conn_buf_y);
                    buffer_compute(compute_flag, kk, l1_buf, conn_buf_x, sum);
                }
            }

            LOOP4:
L13:            for (j = 0; j < 16; j++) {
                l2_buf[j] = (float) (1.0 / (float) (1.0 +
                                                    (float) expf((float) (-(float) sum[0][j] - (float) sum[1][j] -
                                                                          (float) sum[2][j] - (float) sum[3][j] -
                                                                          (float) sum[4][j] - (float) sum[5][j] -
                                                                          (float) sum[6][j] - (float) sum[7][j] -
                                                                          (float) sum[8][j] - (float) sum[9][j] -
                                                                          (float) sum[10][j] - (float) sum[11][j] -
                                                                          (float) sum[12][j] - (float) sum[13][j] -
                                                                          (float) sum[14][j] - (float) sum[15][j]
                                                    ))));
            }

            memcpy_wide_bus_write_float(l2, l2_buf, sizeof(float), sizeof(float) * 16);
}
