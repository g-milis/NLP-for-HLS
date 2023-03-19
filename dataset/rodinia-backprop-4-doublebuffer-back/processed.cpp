for (k = 0; k < TILE_SIZE; k++) {
L2:            LOOP3: for (j = 0; j < 16; j++) {
                new_dw = ETA * delta_buf[j] * ly_buf[k+kk] + MOMENTUM * oldw_buf[k][j];
                w_buf[k][j] += new_dw;
                oldw_buf[k][j] = new_dw;
            }
        }
float w0_buf[16];
float oldw0_buf[16];
float delta_buf[16];
float ly_buf[65536];
float w_buf_x[TILE_SIZE][16];
float w_buf_y[TILE_SIZE][16];
float w_buf_z[TILE_SIZE][16];
float oldw_buf_x[TILE_SIZE][16];
float oldw_buf_y[TILE_SIZE][16];
float oldw_buf_z[TILE_SIZE][16];
for (kk = 0; kk < 65536+2*TILE_SIZE; kk+=TILE_SIZE) {
        int load_flag = ( kk < 65536 );
        int compute_flag = (( kk >= TILE_SIZE ) && ( kk < 65536+TILE_SIZE ));
        int store_flag = ( kk >= 2 * TILE_SIZE );
        i = (kk/TILE_SIZE)%3;

        if (i == 0) {
            buffer_load(load_flag, w+(kk+1)*16, w_buf_x, oldw+(kk+1)*16, oldw_buf_x);
            buffer_compute(compute_flag, kk, delta_buf, ly_buf, w_buf_z, oldw_buf_z);
            buffer_store(store_flag, w+(kk+1)*16-2*TILE_SIZE*16, w_buf_y, oldw+(kk+1)*16-2*TILE_SIZE*16, oldw_buf_y);
        }
        else if (i == 1) {
            buffer_load(load_flag, w+(kk+1)*16, w_buf_y, oldw+(kk+1)*16, oldw_buf_y);
            buffer_compute(compute_flag, kk, delta_buf, ly_buf, w_buf_x, oldw_buf_x);
            buffer_store(store_flag, w+(kk+1)*16-2*TILE_SIZE*16, w_buf_z, oldw+(kk+1)*16-2*TILE_SIZE*16, oldw_buf_z);
        }
        else {
            buffer_load(load_flag, w+(kk+1)*16, w_buf_z, oldw+(kk+1)*16, oldw_buf_z);
            buffer_compute(compute_flag, kk, delta_buf, ly_buf, w_buf_y, oldw_buf_y);
            buffer_store(store_flag, w+(kk+1)*16-2*TILE_SIZE*16, w_buf_x, oldw+(kk+1)*16-2*TILE_SIZE*16, oldw_buf_x);
        }
    }
for (j = 0; j < 16; j++) {
        new_dw = ETA * delta_buf[j] + MOMENTUM * oldw0_buf[j];
        w0_buf[j] += new_dw;
        oldw0_buf[j] = new_dw;
    }
