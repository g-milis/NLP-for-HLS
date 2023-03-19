for (int i(0); i < NUM_PT_IN_BUFFER*NUM_FEATURE; ++i){
		local_searchSpace[i] = searchSpace[start_idx+i];
	}
for (int i = 0; i < NUM_PT_IN_BUFFER; i+=UNROLL_FACTOR) {
L3:        for (int j = 0; j < UNROLL_FACTOR; ++j){
            sum = 0.0;
L4:            for (int k = 0; k < NUM_FEATURE; ++k){
                feature_delta = local_searchSpace[(i+j)*NUM_FEATURE+k] - local_inputQuery[k];
                sum += feature_delta*feature_delta;
            }
            local_distance[i+j] = sum;
        }
	}
for (int i(0); i < NUM_PT_IN_BUFFER; ++i){
        distance[start_idx+i] = local_distance[i];
	}
float local_inputQuery[NUM_FEATURE];
float local_searchSpace[NUM_PT_IN_BUFFER*NUM_FEATURE];
float local_distance[NUM_PT_IN_BUFFER];
for (int i(0); i<NUM_FEATURE; ++i){
		local_inputQuery[i] = inputQuery[i];
    }
for (int tile_idx(0); tile_idx<NUM_TILES; ++tile_idx){
		load(tile_idx, searchSpace, local_searchSpace);
		compute_dist(local_inputQuery, local_searchSpace, local_distance);
        store(tile_idx, local_distance, distance);
	}
