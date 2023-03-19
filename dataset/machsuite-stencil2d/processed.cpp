for (r=0; r<row_size-2; r++) {
L2:        stencil_label2:for (c=0; c<col_size-2; c++) {
            temp = (TYPE)0;
L3:            stencil_label3:for (k1=0;k1<3;k1++){
L4:                stencil_label4:for (k2=0;k2<3;k2++){
                    mul = filter[k1*3 + k2] * orig[(r+k1)*col_size + c+k2];
                    temp += mul;
                }
            }
            sol[(r*col_size) + c] = temp;
        }
    }
