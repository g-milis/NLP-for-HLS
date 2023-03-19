for (k = 0; k < TILE_SIZE; k++) {
L2:            LOOP3: for (j = 0; j < 16; j++) {
                new_dw = (float)ETA * (float)delta_buf[j] * (float)ly_buf[k] + (float)MOMENTUM * (float)oldw_buf[k][j];
                w_buf[k][j] += new_dw;
                oldw_buf[k][j] = new_dw;
            }
        }
float w0_buf[16];
float oldw0_buf[16];
float delta_buf[16];
float ly_buf_x[TILE_SIZE];
float ly_buf_y[TILE_SIZE];
float ly_buf_z[TILE_SIZE];
float w_buf_x[TILE_SIZE][16];
float w_buf_y[TILE_SIZE][16];
float w_buf_z[TILE_SIZE][16];
float oldw_buf_x[TILE_SIZE][16];
float oldw_buf_y[TILE_SIZE][16];
float oldw_buf_z[TILE_SIZE][16];
for(int loop = 0; loop < 1000; ++loop) {
        memcpy_wide_bus_read_float(delta_buf, delta, sizeof(float), sizeof(float) * 16);
                memcpy_wide_bus_read_float(w0_buf, w, 0, sizeof(float) * 16);
        memcpy_wide_bus_read_float(oldw0_buf, oldw, 0, sizeof(float) * 16);

        LOOP1:
L16:        for (kk = 0; kk < 65536 + 2 * TILE_SIZE; kk += TILE_SIZE) {
            int load_flag = (kk < 65536);
            int compute_flag = ((kk >= TILE_SIZE) && (kk < 65536 + TILE_SIZE));
            int store_flag = (kk >= 2 * TILE_SIZE);
            i = (kk / TILE_SIZE) % 3;

            if (i == 0) {
                                buffer_load_w(load_flag, kk, w, w_buf_x);
                buffer_load_oldw(load_flag, kk, oldw, oldw_buf_x);
                buffer_load_ly(load_flag, kk, ly, ly_buf_x);
                buffer_compute(compute_flag, kk, delta_buf, ly_buf_z, w_buf_z, oldw_buf_z);
                                buffer_store_w(store_flag, kk, w, w_buf_y);
                buffer_store_oldw(store_flag, kk, oldw, oldw_buf_y);
            } else if (i == 1) {
                                buffer_load_w(load_flag, kk, w, w_buf_y);
                buffer_load_oldw(load_flag, kk, oldw, oldw_buf_y);
                buffer_load_ly(load_flag, kk, ly, ly_buf_y);
                buffer_compute(compute_flag, kk, delta_buf, ly_buf_x, w_buf_x, oldw_buf_x);
                                buffer_store_w(store_flag, kk, w, w_buf_z);
                buffer_store_oldw(store_flag, kk, oldw, oldw_buf_z);
            } else {
                                buffer_load_w(load_flag, kk, w, w_buf_z);
                buffer_load_oldw(load_flag, kk, oldw, oldw_buf_z);
                buffer_load_ly(load_flag, kk, ly, ly_buf_z);
                buffer_compute(compute_flag, kk, delta_buf, ly_buf_y, w_buf_y, oldw_buf_y);
                                buffer_store_w(store_flag, kk, w, w_buf_x);
                buffer_store_oldw(store_flag, kk , oldw, oldw_buf_x);
            }

        }

        LOOP4:
L17:        for (j = 0; j < 16; j++) {
            new_dw = (float) ETA * (float) delta_buf[j] + (float) MOMENTUM * (float) oldw0_buf[j];
            w0_buf[j] += new_dw;
            oldw0_buf[j] = new_dw;
        }

        memcpy_wide_bus_write_float(w, w0_buf, 0, sizeof(float) * 16);
        memcpy_wide_bus_write_float(oldw, oldw0_buf, 0, sizeof(float) * 16);
    }
