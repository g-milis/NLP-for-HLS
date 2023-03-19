for(int i = 0; i < BUF_SIZE; i++){
						float sum = 0;
L2:			pre_inner:for(int j = 0; j < DIM; j++){
				float a = coord[i * DIM + j] - target[j];
				sum += a * a;
			}
			x_cost[i] = sum * weight[i];
		}
for(int i = 0; i < BUF_SIZE; i++){
			float current_cost = x_cost[i] - cost[i];
			int local_center_index = center_table[assign[i]];
			float work_mem_cost = work_mem[local_center_index] - current_cost;		    
			if(current_cost < 0){
				switch_membership[k + i] = 1;
				cost_of_opening_x[0] += current_cost;
			} else{
				work_mem[local_center_index] = work_mem_cost;
			}
		}
float buffer_target[DIM];
int buffer_center_table[BATCH_SIZE];
float buffer_work_mem[BATCH_SIZE];
char buffer_switch_membership[BATCH_SIZE];
float buffer_cost_of_opening_x[1];
float x_cost[BUF_SIZE];
int buffer_assign_1[BUF_SIZE];
float buffer_cost_1[BUF_SIZE];
float buffer_weight_1[BUF_SIZE];
float buffer_coord_1[BUF_SIZE * DIM];
int buffer_assign_2[BUF_SIZE];
float buffer_cost_2[BUF_SIZE];
float buffer_weight_2[BUF_SIZE];
float buffer_coord_2[BUF_SIZE * DIM];
for(int i = 0; i < BATCH_SIZE + BUF_SIZE; i += BUF_SIZE){
		int load_flag = i >= 0 && i < BATCH_SIZE;
		int compute_flag = i >= BUF_SIZE && i <= BATCH_SIZE + BUF_SIZE;

		if((i/BUF_SIZE) % 2 == 0){
			load(load_flag, i, coord, weight, cost, assign, buffer_coord_1, buffer_weight_1, buffer_cost_1, buffer_assign_1);
			compute(compute_flag, num, i - BUF_SIZE, buffer_coord_2, buffer_weight_2, buffer_target, buffer_cost_2, buffer_assign_2, 
					buffer_center_table, buffer_switch_membership, buffer_cost_of_opening_x, buffer_work_mem, x_cost);
		} else{
			load(load_flag, i, coord, weight, cost, assign, buffer_coord_2, buffer_weight_2, buffer_cost_2, buffer_assign_2);
			compute(compute_flag, num, i - BUF_SIZE, buffer_coord_1, buffer_weight_1, buffer_target, buffer_cost_1, buffer_assign_1, 
					buffer_center_table, buffer_switch_membership, buffer_cost_of_opening_x, buffer_work_mem, x_cost);
		}

	}
