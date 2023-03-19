for (j = 1; j < 17; j++) {
L2:        for (k = 0; k < 65537; k++) {
            new_dw = ((ETA * delta[j] * ly[k]) + (MOMENTUM * oldw[k * 17 + j]));
            w[k * 17 + j] += new_dw;
            oldw[k * 17 + j] = new_dw;
        }
    }
