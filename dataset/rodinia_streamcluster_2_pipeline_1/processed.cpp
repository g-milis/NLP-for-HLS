for(int i = 0; i < BUF_SIZE; i++){
				float sum = 0;
L2:		pre_inner:for(int j = 0; j < DIM; j++){
			float a = coord[i * DIM + j] - target[j];
			sum += a * a;
		}
		x_cost[i] = sum * weight[i];
	}
for(int i = 0; i < BUF_SIZE; i++){
		float current_cost = x_cost[i] - cost[i];
		
		if(current_cost < 0){
			switch_membership[k + i] = 1;
			cost_of_opening_x[0] += current_cost;
		} else{
			int local_center_index = center_table[assign[i]];
			work_mem[local_center_index] -= current_cost;
		}
	}
float buffer_target[DIM];
int buffer_center_table[BATCH_SIZE];
float buffer_work_mem[BATCH_SIZE];
char buffer_switch_membership[BATCH_SIZE];
float buffer_cost_of_opening_x[1];
int buffer_assign[BUF_SIZE];
float buffer_cost[BUF_SIZE];
float buffer_weight[BUF_SIZE];
float buffer_coord[BUF_SIZE * DIM];
float x_cost[BUF_SIZE];
for(int i = 0; i < BATCH_SIZE; i += BUF_SIZE){
		load(i, coord, weight, cost, assign, buffer_coord, buffer_weight, buffer_cost, buffer_assign);
		compute(num, i, buffer_coord, buffer_weight, buffer_target, buffer_cost, buffer_assign, 
				buffer_center_table, buffer_switch_membership, buffer_cost_of_opening_x, buffer_work_mem, x_cost);
	}
