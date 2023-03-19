for( i = 0; i < BATCH_SIZE; i++ ) {
        float sum = 0;
L2:        for( j = 0; j < DIM; j++ ) {
            float a = coord[i * DIM + j] - target[j];
            sum += a * a;
        }
        float current_cost = sum * weight[i] - cost[i];
        int local_center_index = center_table[assign[i]];
        if (current_cost < 0) {

                                          
            switch_membership[i] = 1;
            cost_of_opening_x[0] += current_cost;

        } 
        else {

            
                                                                        work_mem[local_center_index] -= current_cost;
        }
    }
