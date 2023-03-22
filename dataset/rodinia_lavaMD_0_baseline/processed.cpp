TYPE d[POS_DIM];
int neighborOffset[FULL_NEIGHBOR_COUNT][3] = {{-1,-1,-1}, { 0,-1,-1}, { 1,-1,-1}, {-1, 0,-1}, { 0, 0,-1}, { 1, 0,-1}, {-1, 1,-1}, { 0, 1,-1}, { 1, 1,-1}, {-1,-1, 0}, { 0,-1, 0}, { 1,-1, 0}, {-1, 0, 0}, { 0, 0, 0}, { 1, 0, 0}, {-1, 1, 0}, { 0, 1, 0}, { 1, 1, 0}, {-1,-1, 1}, { 0,-1, 1}, { 1,-1, 1}, {-1, 0, 1}, { 0, 0, 1}, { 1, 0, 1}, {-1, 1, 1}, { 0, 1, 1}, { 1, 1, 1}};
for(i=0; i<DIMENSION_3D; i++){
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

L4:        for (l=0; l<FULL_NEIGHBOR_COUNT; ++l){
                        x = x_n + neighborOffset[l][0];
            y = y_n + neighborOffset[l][1];
            z = z_n + neighborOffset[l][2];
                        B_idx = z*DIMENSION_2D_PADDED + y*DIMENSION_1D_PADDED + x;
            B_idx = B_idx * NUMBER_PAR_PER_BOX;

            L5:            PARTICLES_A: for(j=0; j<NUMBER_PAR_PER_BOX; ++j){
                A_idx_j = A_idx + j;
                C_idx_j = C_idx + j;
L6:                PARTICLES_B: for(k=0; k<NUMBER_PAR_PER_BOX; ++k){
                    B_idx_k = B_idx + k;
                                        r2 = pos_i[A_idx_j*POS_DIM+V] + pos_i[B_idx_k*POS_DIM+V] -
                        (pos_i[A_idx_j*POS_DIM+X] * pos_i[B_idx_k*POS_DIM+X] +
                         pos_i[A_idx_j*POS_DIM+Y] * pos_i[B_idx_k*POS_DIM+Y] +
                         pos_i[A_idx_j*POS_DIM+Z] * pos_i[B_idx_k*POS_DIM+Z]);
                    u2 = A2 * r2;
                                        u2 = u2 * -1.0;
                    vij = 1.0 + (u2) + 0.5*((u2)*(u2)) +
                            0.16666*((u2)*(u2)*(u2)) +
                            0.041666*((u2)*(u2)*(u2)*(u2));
                    fs = 2. * vij;
                    d[X] = pos_i[A_idx_j*POS_DIM+X] - pos_i[B_idx_k*POS_DIM+X];
                    d[Y] = pos_i[A_idx_j*POS_DIM+Y] - pos_i[B_idx_k*POS_DIM+Y];
                    d[Z] = pos_i[A_idx_j*POS_DIM+Z] - pos_i[B_idx_k*POS_DIM+Z];
                    fxij = fs * d[X];
                    fyij = fs * d[Y];
                    fzij = fs * d[Z];
                                        pos_o[C_idx_j*POS_DIM+V] += q_i[B_idx_k] * vij;
                    pos_o[C_idx_j*POS_DIM+X] += q_i[B_idx_k] * fxij;
                    pos_o[C_idx_j*POS_DIM+Y] += q_i[B_idx_k] * fyij;
                    pos_o[C_idx_j*POS_DIM+Z] += q_i[B_idx_k] * fzij;
                }
            }
        }
    }
