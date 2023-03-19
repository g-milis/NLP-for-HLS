bool strel[25] = { 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };
for (int i = 0; i < GRID_ROWS; i++) {
L3:		for (int j = 0; j < GRID_COLS; j++) {
			float max = 0.0, temp;
L4:			for (int m = 0; m < STREL_ROWS; m++) {
L5:				for (int n = 0; n < STREL_COLS; n++) {
					int p = i - radius_p + m;
					int q = j - radius_q + n;
					if (p >= 0 && q >= 0 && p < GRID_ROWS && q < GRID_COLS && strel[m * STREL_COLS + n] != 0) {
						temp = img[starting_idx + p * GRID_COLS + q];
						if (temp > max) max = temp;
					}
				}
			}
			result[i * GRID_COLS + j] = max;
		}
	}
