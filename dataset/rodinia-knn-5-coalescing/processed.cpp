for (int i(0); i < NUM_PT_IN_BUFFER*NUM_FEATURE/WIDTH_FACTOR; ++i){
            local_searchSpace[i] = searchSpace[start_idx+i];
        }
for (int i = 0; i < NUM_PT_IN_BUFFER*NUM_FEATURE/WIDTH_FACTOR; i+=UNROLL_FACTOR){
L3:            for (int n = 0; n < UNROLL_FACTOR; ++n){
L4:                for (int j = 0; j < WIDTH_FACTOR; j+=NUM_FEATURE){
                    float feature_delta = 0.0;
                    float sum = 0.0;
L5:                    for (int k = 0; k < NUM_FEATURE; ++k){
                        unsigned int range_idx = (j+k) * 32;
                        uint32_t feature_item = local_searchSpace[i+n].range(range_idx+31, range_idx);
                        float feature_item_value = *((float*)(&feature_item));
                        feature_delta = feature_item_value - local_inputQuery[k];
                        sum += feature_delta * feature_delta;
                    }
                    unsigned int range_idx = (((i+n)%2)*WIDTH_FACTOR/2+ j/2) * 32;
                    local_distance[(i+n)/2].range(range_idx+31, range_idx) = *((uint32_t *)(&sum));
                }
            }
        }
for (int i(0); i < NUM_PT_IN_BUFFER/WIDTH_FACTOR; ++i){
            distance[start_idx+i] = local_distance[i];
        }
float local_inputQuery[NUM_FEATURE];
INTERFACE_WIDTH local_searchSpace_0[NUM_PT_IN_BUFFER*NUM_FEATURE/WIDTH_FACTOR];
INTERFACE_WIDTH local_searchSpace_1[NUM_PT_IN_BUFFER*NUM_FEATURE/WIDTH_FACTOR];
INTERFACE_WIDTH local_distance_0[NUM_PT_IN_BUFFER/WIDTH_FACTOR];
INTERFACE_WIDTH local_distance_1[NUM_PT_IN_BUFFER/WIDTH_FACTOR];
for (int i(0); i<NUM_FEATURE; ++i){
		local_inputQuery[i] = inputQuery[i];
    }
for (int tile_idx(0); tile_idx<NUM_TILES+2; ++tile_idx){
		int load_flag = tile_idx >= 0 && tile_idx < NUM_TILES;
		int compute_flag = tile_idx >= 1 && tile_idx < NUM_TILES + 1;
		int store_flag = tile_idx >= 2 && tile_idx < NUM_TILES + 2;

	    if (tile_idx % 2 == 0) {
	    	load(load_flag, tile_idx, searchSpace, local_searchSpace_0);
			compute(compute_flag, local_inputQuery, local_searchSpace_1, local_distance_1);
			store(store_flag, tile_idx-2, local_distance_0, distance);
	    }
	    else {
	    	load(load_flag, tile_idx, searchSpace, local_searchSpace_1);
			compute(compute_flag, local_inputQuery, local_searchSpace_0, local_distance_0);
			store(store_flag, tile_idx-2, local_distance_1, distance);
	    }
	}
