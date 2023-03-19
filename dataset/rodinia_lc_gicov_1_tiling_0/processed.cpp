for (i = 0; i < GRID_COLS; i++) {
        int j;

L2:        for (j = MAX_RADIUS; j < TILE_ROWS + 2 * MAX_RADIUS - MAX_RADIUS; j++) {
            if (i < MAX_RADIUS || i >= GRID_COLS - MAX_RADIUS || (which_boundary == TOP && j < MAX_RADIUS + MAX_RADIUS) || (which_boundary == BOTTOM && j >= TILE_ROWS + 2 * MAX_RADIUS - MAX_RADIUS - MAX_RADIUS)) {
                result[j * GRID_COLS + i - MAX_RADIUS * GRID_COLS] = 0;
                continue;
            }
                        int k, n;
L3:            float Grad[NPOINTS];
            float max_GICOV = 0;

            L4:            for (k = 0; k < NCIRCLES; k++) {
                L5:                for (n = 0; n < NPOINTS; n++) {
                    int x, y;
                                        y = j + tY[k][n];
                    x = i + tX[k][n];

                                        Grad[n] = grad_x[y * GRID_COLS + x] * cos_angle[n] + grad_y[y * GRID_COLS + x] * sin_angle[n];
                }

                                float sum = 0.0;
L6:                for (n = 0; n < NPOINTS; n++) sum += Grad[n];
                float mean = sum / (float)NPOINTS;

                                float var = 0.0;
L7:                for (n = 0; n < NPOINTS; n++) {
                    sum = Grad[n] - mean;
                    var += sum * sum;
                }
                var = var / (float)(NPOINTS - 1);

                                if (mean * mean / var > max_GICOV) {
                    result[j * GRID_COLS + i - MAX_RADIUS * GRID_COLS] = mean / sqrt(var);
                    max_GICOV = mean * mean / var;
                }
            }
        }
    }
