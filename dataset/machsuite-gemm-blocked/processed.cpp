for (jj = 0; jj < row_size; jj += block_size){
L2:        loopkk:for (kk = 0; kk < row_size; kk += block_size){
L3:            loopi:for ( i = 0; i < row_size; ++i){
L4:                loopk:for (k = 0; k < block_size; ++k){
                    i_row = i * row_size;
                    k_row = (k  + kk) * row_size;
                    temp_x = m1[i_row + k + kk];
L5:                    loopj:for (j = 0; j < block_size; ++j){
                        mul = temp_x * m2[k_row + j + jj];
                        prod[i_row + j + jj] += mul;
                    }
                }
            }
        }
    }
