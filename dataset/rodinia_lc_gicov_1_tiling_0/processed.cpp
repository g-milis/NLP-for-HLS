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
float sin_angle[16] = {0, 0.382683432365090, 0.707106781186548, 0.923879532511287, 1, 0.923879532511287, 0.707106781186548, 0.382683432365090, 1.22464679914735e-16, -0.382683432365090, -0.707106781186548, -0.923879532511287, -1, -0.923879532511287, -0.707106781186548, -0.382683432365090};
float cos_angle[16] = {1, 0.923879532511287, 0.707106781186548, 0.382683432365090, 6.12323399573677e-17, -0.382683432365090, -0.707106781186548, -0.923879532511287, -1, -0.923879532511287, -0.707106781186548, -0.382683432365090, -1.83697019872103e-16, 0.382683432365090, 0.707106781186547, 0.923879532511287};
int tX[2][16] = {{1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0}, {2, 1, 1, 0, 0, -1, -2, -2, -2, -2, -2, -1, 0, 0, 1, 1}};
int tY[2][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1}, {0, 0, 1, 1, 1, 1, 1, 0, 0, -1, -2, -2, -2, -2, -2, -1}};
float result_inner [TILE_ROWS * GRID_COLS];
float grad_x_inner [(TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS];
float grad_y_inner [(TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS];
for (k = 0; k < GRID_ROWS / TILE_ROWS; k++) {
        memcpy(grad_x_inner, grad_x + k * TILE_ROWS * GRID_COLS - MAX_RADIUS * GRID_COLS, sizeof(float) * (TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS);
        memcpy(grad_y_inner, grad_y + k * TILE_ROWS * GRID_COLS - MAX_RADIUS * GRID_COLS, sizeof(float) * (TILE_ROWS + 2 * MAX_RADIUS) * GRID_COLS);
        
        lc_gicov(result_inner, grad_x_inner, grad_y_inner, tX, tY, sin_angle, cos_angle, k);
        
        memcpy(result + k * TILE_ROWS * GRID_COLS, result_inner, sizeof(float) * (TILE_ROWS * GRID_COLS));
    }
