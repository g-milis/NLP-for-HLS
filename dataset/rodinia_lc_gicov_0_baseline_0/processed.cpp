float sin_angle[16] = {0, 0.382683432365090, 0.707106781186548, 0.923879532511287, 1, 0.923879532511287, 0.707106781186548, 0.382683432365090, 1.22464679914735e-16, -0.382683432365090, -0.707106781186548, -0.923879532511287, -1, -0.923879532511287, -0.707106781186548, -0.382683432365090};
float cos_angle[16] = {1, 0.923879532511287, 0.707106781186548, 0.382683432365090, 6.12323399573677e-17, -0.382683432365090, -0.707106781186548, -0.923879532511287, -1, -0.923879532511287, -0.707106781186548, -0.382683432365090, -1.83697019872103e-16, 0.382683432365090, 0.707106781186547, 0.923879532511287};
int tX[2][16] = {{1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0}, {2, 1, 1, 0, 0, -1, -2, -2, -2, -2, -2, -1, 0, 0, 1, 1}};
int tY[2][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1}, {0, 0, 1, 1, 1, 1, 1, 0, 0, -1, -2, -2, -2, -2, -2, -1}};
for (i = MAX_RADIUS; i < GRID_COLS - MAX_RADIUS; i++) {
L6:		float Grad[NPOINTS];
		int j, k, n, x, y;

L7:		for (j = MAX_RADIUS; j < GRID_ROWS - MAX_RADIUS; j++) {
						float max_GICOV = 0;

			L8:			for (k = 0; k < NCIRCLES; k++) {
				L9:				for (n = 0; n < NPOINTS; n++) {
										y = j + tY[k][n];
					x = i + tX[k][n];

										Grad[n] = grad_x[y * GRID_COLS + x] * cos_angle[n] + grad_y[y * GRID_COLS + x] * sin_angle[n];
				}

								float sum = 0.0;
L10:				for (n = 0; n < NPOINTS; n++) sum += Grad[n];
				float mean = sum / (float)NPOINTS;

								float var = 0.0;
L11:				for (n = 0; n < NPOINTS; n++) {
					sum = Grad[n] - mean;
					var += sum * sum;
				}
				var = var / (float)(NPOINTS - 1);

								if (mean * mean / var > max_GICOV) {
					result[j * GRID_COLS + i] = mean / sqrt(var);
					max_GICOV = mean * mean / var;
				}
			}
		}
	}
