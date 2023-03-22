DTYPE in_local[BATCH_SIZE_1];
DTYPE out_local[BATCH_SIZE_1];
for(int i = 0; i < BATCH_SIZE_1 / FLOATS_PER_ELEMENT; i++) {
			temp = in[i];

L4:			for(int j = 0; j < FLOATS_PER_ELEMENT; j++) {
				raw_float t;
			        t.raw_val = temp.range((j + 1) * FLOAT_BITS - 1, j * FLOAT_BITS);
			        in_local[counter] = (DTYPE)t.float_val;

				counter++;
			}
		}
DTYPE u_hat_arr[INTERNAL_BATCHES];
DTYPE p_arr_1[INTERNAL_BATCHES];
DTYPE k_arr[INTERNAL_BATCHES];
DTYPE calc_temp_arr[INTERNAL_BATCHES];
for (int i = 0; i < INTERNAL_BATCHES; i++) {
			u_hat_arr[i] = in_local[i * INTERNAL_BATCH_SIZE];
			p_arr_1[i] = 0.5;
			out_local[i * INTERNAL_BATCH_SIZE] = u_hat_arr[i];
		}
for (int t = 1; t < INTERNAL_BATCH_SIZE; t++) {
L11:			for (int i = 0; i < INTERNAL_BATCHES; i++) {
				calc_temp_arr[i] = p_arr_1[i] + (DTYPE)0.01;

				k_arr[i] = calc_temp_arr[i] / (p_arr_1[i] + (DTYPE)1.01);
				
				u_hat_arr[i] = u_hat_arr[i] + k_arr[i] * (in_local[i * INTERNAL_BATCH_SIZE + t] - u_hat_arr[i]);
				
				p_arr_1[i] = ((DTYPE)1 - k_arr[i]) * calc_temp_arr[i];
				
				out_local[i * INTERNAL_BATCH_SIZE + t] = u_hat_arr[i];
			}
		}
for(int i = 0; i < BATCH_SIZE_1 / FLOATS_PER_ELEMENT; i++) {
			temp = 0;

L13:		for(int j = 0; j < FLOATS_PER_ELEMENT; j++) {
				raw_float t;
				t.float_val = (float)out_local[counter];

 				temp.range((j + 1) * FLOAT_BITS - 1, j * FLOAT_BITS) = t.raw_val;

				counter++;
			}

			out[i] = temp;
		}
