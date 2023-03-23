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
for(tid = 0; tid < THREADS; tid++){
                        data_x[0] = work_x[0*stride+tid];
            data_x[1] = work_x[1*stride+tid];
            data_x[2] = work_x[2*stride+tid];
            data_x[3] = work_x[3*stride+tid];
            data_x[4] = work_x[4*stride+tid];
            data_x[5] = work_x[5*stride+tid];
            data_x[6] = work_x[6*stride+tid];
            data_x[7] = work_x[7*stride+tid];

            data_y[0] = work_y[0*stride+tid];
            data_y[1] = work_y[1*stride+tid];
            data_y[2] = work_y[2*stride+tid];
            data_y[3] = work_y[3*stride+tid];
            data_y[4] = work_y[4*stride+tid];
            data_y[5] = work_y[5*stride+tid];
            data_y[6] = work_y[6*stride+tid];
            data_y[7] = work_y[7*stride+tid];

                        FFT8(data_x, data_y);

                        twiddles8(data_x, data_y, tid, 512);

                        DATA_x[tid*8]     = data_x[0];
            DATA_x[tid*8 + 1] = data_x[1];
            DATA_x[tid*8 + 2] = data_x[2];
            DATA_x[tid*8 + 3] = data_x[3];
            DATA_x[tid*8 + 4] = data_x[4];
            DATA_x[tid*8 + 5] = data_x[5];
            DATA_x[tid*8 + 6] = data_x[6];
            DATA_x[tid*8 + 7] = data_x[7];

            DATA_y[tid*8]     = data_y[0];
            DATA_y[tid*8 + 1] = data_y[1];
            DATA_y[tid*8 + 2] = data_y[2];
            DATA_y[tid*8 + 3] = data_y[3];
            DATA_y[tid*8 + 4] = data_y[4];
            DATA_y[tid*8 + 5] = data_y[5];
            DATA_y[tid*8 + 6] = data_y[6];
            DATA_y[tid*8 + 7] = data_y[7];
        }
for(tid = 0; tid < 64; tid++){
            hi = tid>>3;
            lo = tid&7;
            offset = hi*8+lo;
            smem[0*sx+offset] = DATA_x[tid*8 + 0];
            smem[4*sx+offset] = DATA_x[tid*8 + 1];
            smem[1*sx+offset] = DATA_x[tid*8 + 4];
            smem[5*sx+offset] = DATA_x[tid*8 + 5];
            smem[2*sx+offset] = DATA_x[tid*8 + 2];
            smem[6*sx+offset] = DATA_x[tid*8 + 3];
            smem[3*sx+offset] = DATA_x[tid*8 + 6];
            smem[7*sx+offset] = DATA_x[tid*8 + 7];
        }
for(tid = 0; tid < 64; tid++){
            hi = tid>>3;
            lo = tid&7;
            offset = lo*66+hi;

            DATA_x[tid*8 +0] = smem[0*sx+offset];
            DATA_x[tid*8 +4] = smem[4*sx+offset];
            DATA_x[tid*8 +1] = smem[1*sx+offset];
            DATA_x[tid*8 +5] = smem[5*sx+offset];
            DATA_x[tid*8 +2] = smem[2*sx+offset];
            DATA_x[tid*8 +6] = smem[6*sx+offset];
            DATA_x[tid*8 +3] = smem[3*sx+offset];
            DATA_x[tid*8 +7] = smem[7*sx+offset];
        }
for(tid = 0; tid < 64; tid++){
            hi = tid>>3;
            lo = tid&7;
            offset = hi*8+lo;

            smem[0*sx+offset] = DATA_y[tid*8 + 0];
            smem[4*sx+offset] = DATA_y[tid*8 + 1];
            smem[1*sx+offset] = DATA_y[tid*8 + 4];
            smem[5*sx+offset] = DATA_y[tid*8 + 5];
            smem[2*sx+offset] = DATA_y[tid*8 + 2];
            smem[6*sx+offset] = DATA_y[tid*8 + 3];
            smem[3*sx+offset] = DATA_y[tid*8 + 6];
            smem[7*sx+offset] = DATA_y[tid*8 + 7];
        }
for(tid = 0; tid < 64; tid++){
            data_y[0] = DATA_y[tid*8 + 0];
            data_y[1] = DATA_y[tid*8 + 1];
            data_y[2] = DATA_y[tid*8 + 2];
            data_y[3] = DATA_y[tid*8 + 3];
            data_y[4] = DATA_y[tid*8 + 4];
            data_y[5] = DATA_y[tid*8 + 5];
            data_y[6] = DATA_y[tid*8 + 6];
            data_y[7] = DATA_y[tid*8 + 7];

            hi = tid>>3;
            lo = tid&7;

            loady8(data_y, smem, lo*66+hi, 8);

            DATA_y[tid*8]     = data_y[0];
            DATA_y[tid*8 + 1] = data_y[1];
            DATA_y[tid*8 + 2] = data_y[2];
            DATA_y[tid*8 + 3] = data_y[3];
            DATA_y[tid*8 + 4] = data_y[4];
            DATA_y[tid*8 + 5] = data_y[5];
            DATA_y[tid*8 + 6] = data_y[6];
            DATA_y[tid*8 + 7] = data_y[7];
        }
for(tid = 0; tid < 64; tid++){
            data_x[0] = DATA_x[tid*8 + 0];
            data_x[1] = DATA_x[tid*8 + 1];
            data_x[2] = DATA_x[tid*8 + 2];
            data_x[3] = DATA_x[tid*8 + 3];
            data_x[4] = DATA_x[tid*8 + 4];
            data_x[5] = DATA_x[tid*8 + 5];
            data_x[6] = DATA_x[tid*8 + 6];
            data_x[7] = DATA_x[tid*8 + 7];

            data_y[0] = DATA_y[tid*8 + 0];
            data_y[1] = DATA_y[tid*8 + 1];
            data_y[2] = DATA_y[tid*8 + 2];
            data_y[3] = DATA_y[tid*8 + 3];
            data_y[4] = DATA_y[tid*8 + 4];
            data_y[5] = DATA_y[tid*8 + 5];
            data_y[6] = DATA_y[tid*8 + 6];
            data_y[7] = DATA_y[tid*8 + 7];

                        FFT8(data_x, data_y);

                        hi = tid>>3;

                        twiddles8(data_x, data_y, hi, 64);

                        DATA_x[tid*8]     = data_x[0];
            DATA_x[tid*8 + 1] = data_x[1];
            DATA_x[tid*8 + 2] = data_x[2];
            DATA_x[tid*8 + 3] = data_x[3];
            DATA_x[tid*8 + 4] = data_x[4];
            DATA_x[tid*8 + 5] = data_x[5];
            DATA_x[tid*8 + 6] = data_x[6];
            DATA_x[tid*8 + 7] = data_x[7];

            DATA_y[tid*8]     = data_y[0];
            DATA_y[tid*8 + 1] = data_y[1];
            DATA_y[tid*8 + 2] = data_y[2];
            DATA_y[tid*8 + 3] = data_y[3];
            DATA_y[tid*8 + 4] = data_y[4];
            DATA_y[tid*8 + 5] = data_y[5];
            DATA_y[tid*8 + 6] = data_y[6];
            DATA_y[tid*8 + 7] = data_y[7];
        }
for(tid = 0; tid < 64; tid++){
            hi = tid>>3;
            lo = tid&7;
            offset = hi*72+lo;

            DATA_x[tid*8 +0] = smem[0*sx+offset];
            DATA_x[tid*8 +4] = smem[4*sx+offset];
            DATA_x[tid*8 +1] = smem[1*sx+offset];
            DATA_x[tid*8 +5] = smem[5*sx+offset];
            DATA_x[tid*8 +2] = smem[2*sx+offset];
            DATA_x[tid*8 +6] = smem[6*sx+offset];
            DATA_x[tid*8 +3] = smem[3*sx+offset];
            DATA_x[tid*8 +7] = smem[7*sx+offset];
        }
for(tid = 0; tid < 64; tid++){
             data_y[0] = DATA_y[tid*8 + 0];
             data_y[1] = DATA_y[tid*8 + 1];
             data_y[2] = DATA_y[tid*8 + 2];
             data_y[3] = DATA_y[tid*8 + 3];
             data_y[4] = DATA_y[tid*8 + 4];
             data_y[5] = DATA_y[tid*8 + 5];
             data_y[6] = DATA_y[tid*8 + 6];
             data_y[7] = DATA_y[tid*8 + 7];

             hi = tid>>3;
             lo = tid&7;

             loady8(data_y, smem, hi*72+lo, 8);

             DATA_y[tid*8 + 0] = data_y[0];
             DATA_y[tid*8 + 1] = data_y[1];
             DATA_y[tid*8 + 2] = data_y[2];
             DATA_y[tid*8 + 3] = data_y[3];
             DATA_y[tid*8 + 4] = data_y[4];
             DATA_y[tid*8 + 5] = data_y[5];
             DATA_y[tid*8 + 6] = data_y[6];
             DATA_y[tid*8 + 7] = data_y[7];
         }
for(tid = 0; tid < 64; tid++){
                          data_y[0] = DATA_y[tid*8];
             data_y[1] = DATA_y[tid*8 + 1];
             data_y[2] = DATA_y[tid*8 + 2];
             data_y[3] = DATA_y[tid*8 + 3];
             data_y[4] = DATA_y[tid*8 + 4];
             data_y[5] = DATA_y[tid*8 + 5];
             data_y[6] = DATA_y[tid*8 + 6];
             data_y[7] = DATA_y[tid*8 + 7];

             data_x[0] = DATA_x[tid*8];
             data_x[1] = DATA_x[tid*8 + 1];
             data_x[2] = DATA_x[tid*8 + 2];
             data_x[3] = DATA_x[tid*8 + 3];
             data_x[4] = DATA_x[tid*8 + 4];
             data_x[5] = DATA_x[tid*8 + 5];
             data_x[6] = DATA_x[tid*8 + 6];
             data_x[7] = DATA_x[tid*8 + 7];

                          FFT8(data_x, data_y);

                          work_x[0*stride+tid] = data_x[reversed[0]];
             work_x[1*stride+tid] = data_x[reversed[1]];
             work_x[2*stride+tid] = data_x[reversed[2]];
             work_x[3*stride+tid] = data_x[reversed[3]];
             work_x[4*stride+tid] = data_x[reversed[4]];
             work_x[5*stride+tid] = data_x[reversed[5]];
             work_x[6*stride+tid] = data_x[reversed[6]];
             work_x[7*stride+tid] = data_x[reversed[7]];

             work_y[0*stride+tid] = data_y[reversed[0]];
             work_y[1*stride+tid] = data_y[reversed[1]];
             work_y[2*stride+tid] = data_y[reversed[2]];
             work_y[3*stride+tid] = data_y[reversed[3]];
             work_y[4*stride+tid] = data_y[reversed[4]];
             work_y[5*stride+tid] = data_y[reversed[5]];
             work_y[6*stride+tid] = data_y[reversed[6]];
             work_y[7*stride+tid] = data_y[reversed[7]];
         }
