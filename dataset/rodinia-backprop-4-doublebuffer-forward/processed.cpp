for (k = 0; k < TILE_SIZE; k++) {
L2:            LOOP3: for (j = 0; j < 16; j++) {
                float product = conn_buf[k][j] * l1_buf[k + kk];
                sum[j] += product;
            }
        }
