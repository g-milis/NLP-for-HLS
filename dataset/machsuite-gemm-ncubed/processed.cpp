for(i=0;i<row_size;i++) {
L2:        middle:for(j=0;j<col_size;j++) {
            i_col = i * col_size;
            TYPE sum = 0;
L3:            inner:for(k=0;k<row_size;k++) {
                k_col = k * col_size;
                mult = m1[i_col + k] * m2[k_col + j];
                sum += mult;
            }
            prod[i_col + j]  = sum;
        }
    }
