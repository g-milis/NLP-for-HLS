for (k = 0; k < TILE_SIZE; k++) {
L2:            LOOP3: for (j = 0; j < 16; j++) {
                float product = conn_buf[k][j] * l1_buf[k + kk];
                sum[j] += product;
            }
        }
float sum[16];
float l1_buf[65536];
float conn_buf_x[TILE_SIZE][16];
float conn_buf_y[TILE_SIZE][16];
float l2_buf[16];
for (kk = 0; kk < 65536+TILE_SIZE; kk += TILE_SIZE) {
        int load_flag = ( kk < 65536 );
        int compute_flag = ( kk >= TILE_SIZE );
        i = (kk/TILE_SIZE)%2;

        if (i == 0) {
            buffer_load(load_flag, conn+(kk+1)*16, conn_buf_x);
            buffer_compute(compute_flag, kk, l1_buf, conn_buf_y, sum);
        }
        else {
            buffer_load(load_flag, conn+(kk+1)*16, conn_buf_y);
            buffer_compute(compute_flag, kk, l1_buf, conn_buf_x, sum);
        }
    }
for (j = 0; j < 16; j++) {
        l2_buf[j] = (1.0 / (1.0 + exp(-sum[j])));
    }
