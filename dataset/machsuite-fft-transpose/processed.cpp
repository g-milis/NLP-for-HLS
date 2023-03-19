int reversed8[8] = {0,4,2,6,1,5,3,7};
for(j=1; j < 8; j++){
        phi = ((-2*PI*reversed8[j]/n)*i);
        phi_x = cos(phi);
        phi_y = sin(phi);
        tmp = a_x[j];
        a_x[j] = cmplx_M_x(a_x[j], a_y[j], phi_x, phi_y);
        a_y[j] = cmplx_M_y(tmp, a_y[j], phi_x, phi_y);
    }
int reversed[8] = {0,4,2,6,1,5,3,7};
TYPE DATA_x[THREADS*8];
TYPE DATA_y[THREADS*8];
TYPE data_x[ 8 ];
TYPE data_y[ 8 ];
TYPE smem[8*8*9];
