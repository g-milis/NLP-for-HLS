TYPE r2[UNROLL_SIZE];
TYPE u2[UNROLL_SIZE];
TYPE fs[UNROLL_SIZE];
TYPE vij[UNROLL_SIZE];
TYPE fxij[UNROLL_SIZE];
TYPE fyij[UNROLL_SIZE];
TYPE fzij[UNROLL_SIZE];
TYPE d[UNROLL_SIZE][POS_DIM];
int neighborOffset[FULL_NEIGHBOR_COUNT][3] = {{-1,-1,-1}, { 0,-1,-1}, { 1,-1,-1}, {-1, 0,-1}, { 0, 0,-1}, { 1, 0,-1}, {-1, 1,-1}, { 0, 1,-1}, { 1, 1,-1}, {-1,-1, 0}, { 0,-1, 0}, { 1,-1, 0}, {-1, 0, 0}, { 0, 0, 0}, { 1, 0, 0}, {-1, 1, 0}, { 0, 1, 0}, { 1, 1, 0}, {-1,-1, 1}, { 0,-1, 1}, { 1,-1, 1}, {-1, 0, 1}, { 0, 0, 1}, { 1, 0, 1}, {-1, 1, 1}, { 0, 1, 1}, { 1, 1, 1}};
TYPE local_A_pos_i[NUMBER_PAR_PER_BOX][POS_DIM];
TYPE local_B_pos_i[NUMBER_PAR_PER_BOX][POS_DIM];
TYPE local_B_q_i[NUMBER_PAR_PER_BOX];
TYPE local_pos_o[NUMBER_PAR_PER_BOX][POS_DIM];
for(i=0; i<DIMENSION_3D; i++){
        L15:        WIPE_LOCAL_POS_O: for (ii=0; ii<NUMBER_PAR_PER_BOX; ++ii){
            local_pos_o[ii][V] = 0.0;
            local_pos_o[ii][X] = 0.0;
            local_pos_o[ii][Y] = 0.0;
            local_pos_o[ii][Z] = 0.0;
        }
                C_idx = i * NUMBER_PAR_PER_BOX;
                z = i / DIMENSION_2D;
        remainder = i % DIMENSION_2D;
        y = remainder / DIMENSION_1D;
        x = remainder % DIMENSION_1D;
                x_n = x + 1;
        y_n = y + 1;
        z_n = z + 1;
                A_idx = z_n*DIMENSION_2D_PADDED + y_n*DIMENSION_1D_PADDED + x_n;
        A_idx = A_idx * NUMBER_PAR_PER_BOX;
        L16:        LOAD_LOCAL_A_POS_I: for (ii=0; ii<NUMBER_PAR_PER_BOX; ++ii){
            local_A_pos_i[ii][V] = pos_i[(A_idx+ii)*POS_DIM+V];
            local_A_pos_i[ii][X] = pos_i[(A_idx+ii)*POS_DIM+X];
            local_A_pos_i[ii][Y] = pos_i[(A_idx+ii)*POS_DIM+Y];
            local_A_pos_i[ii][Z] = pos_i[(A_idx+ii)*POS_DIM+Z];
        }
        L17:        for (l=0; l<FULL_NEIGHBOR_COUNT; ++l){
                        x = x_n + neighborOffset[l][0];
            y = y_n + neighborOffset[l][1];
            z = z_n + neighborOffset[l][2];
                        B_idx = z*DIMENSION_2D_PADDED + y*DIMENSION_1D_PADDED + x;
            B_idx = B_idx * NUMBER_PAR_PER_BOX;
            L18:            LOAD_LOCAL_B_POS_I: for (ii=0; ii<NUMBER_PAR_PER_BOX; ++ii){
                local_B_pos_i[ii][V] = pos_i[(B_idx+ii)*POS_DIM+V];
                local_B_pos_i[ii][X] = pos_i[(B_idx+ii)*POS_DIM+X];
                local_B_pos_i[ii][Y] = pos_i[(B_idx+ii)*POS_DIM+Y];
                local_B_pos_i[ii][Z] = pos_i[(B_idx+ii)*POS_DIM+Z];
            }
            L19:            LOAD_LOCAL_B_Q_I: for (ii=0; ii<NUMBER_PAR_PER_BOX; ++ii){
                local_B_q_i[ii] = q_i[B_idx+ii];
            }
            L20:            PARTICLES_B: for(k=0; k<NUMBER_PAR_PER_BOX; ++k){
L21:                PARTICLES_A: for(j=0; j<NUMBER_PAR_PER_BOX; j+=UNROLL_SIZE){
L22:                    UNROLLING_LOOP: for(jj=0; jj<UNROLL_SIZE; ++jj){
                                                r2[jj] = local_A_pos_i[j+jj][V] + local_B_pos_i[k][V] -
                            (local_A_pos_i[j+jj][X] * local_B_pos_i[k][X] +
                             local_A_pos_i[j+jj][Y] * local_B_pos_i[k][Y] +
                             local_A_pos_i[j+jj][Z] * local_B_pos_i[k][Z]);                         u2[jj] = A2 * r2[jj];
                                                u2[jj] = u2[jj] * -1.0;
                        vij[jj] = 1.0 + (u2[jj]) + 0.5*((u2[jj])*(u2[jj])) +
                                0.16666*((u2[jj])*(u2[jj])*(u2[jj])) +
                                0.041666*((u2[jj])*(u2[jj])*(u2[jj])*(u2[jj]));
                        fs[jj] = 2. * vij[jj];
                        d[jj][X] = local_A_pos_i[j+jj][X] - local_B_pos_i[k][X];
                        d[jj][Y] = local_A_pos_i[j+jj][Y] - local_B_pos_i[k][Y];
                        d[jj][Z] = local_A_pos_i[j+jj][Z] - local_B_pos_i[k][Z];
                        fxij[jj] = fs[jj] * d[jj][X];
                        fyij[jj] = fs[jj] * d[jj][Y];
                        fzij[jj] = fs[jj] * d[jj][Z];
                                                local_pos_o[j+jj][V] += local_B_q_i[k] * vij[jj];
                        local_pos_o[j+jj][X] += local_B_q_i[k] * fxij[jj];
                        local_pos_o[j+jj][Y] += local_B_q_i[k] * fyij[jj];
                        local_pos_o[j+jj][Z] += local_B_q_i[k] * fzij[jj];
                    }
                }
            }
        }
        L23:        WRITE_POS_O: for (ii=0; ii<NUMBER_PAR_PER_BOX; ++ii){
            pos_o[(C_idx+ii)*POS_DIM+V] = local_pos_o[ii][V];
            pos_o[(C_idx+ii)*POS_DIM+X] = local_pos_o[ii][X];
            pos_o[(C_idx+ii)*POS_DIM+Y] = local_pos_o[ii][Y];
            pos_o[(C_idx+ii)*POS_DIM+Z] = local_pos_o[ii][Z];
        }
    }
