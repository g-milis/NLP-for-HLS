for (int i(0); i<NCLUSTERS; ++i){
L2:		for (int j(0); j<NFEATURES; ++j){
			local_clusters[i*NFEATURES+j] = clusters[i*NFEATURES+j];
		}
	}
for (int i(0); i<TILE_SIZE; ++i){
L4:		for (int j(0); j<NFEATURES; ++j){
			local_feature[i*NFEATURES+j] = feature[(tile_idx*TILE_SIZE+i)*NFEATURES+j];
		}
	}
for (int i = 0; i < TILE_SIZE; i++) {
    	float min_dist = FLT_MAX;
        int index = 0;

        
L6:        MIN: for (int j = 0; j < NCLUSTERS; j++) {
        	float dist = 0.0;

L7:            DIST: for (int k = 0; k < NFEATURES; k++) {
                float diff = local_feature[NFEATURES * i + k] - local_clusters[NFEATURES * j + k];
                dist += diff * diff;
            }

            if (dist < min_dist) {
                min_dist = dist;
                index = j;
            }
        }

        
        local_membership[i] = index;
    }
for (int i(0); i<TILE_SIZE; ++i){
		membership[tile_idx*TILE_SIZE+i] = local_membership[i];
	}
int local_membership_0[TILE_SIZE];
float local_feature_0[TILE_SIZE * NFEATURES];
int local_membership_1[TILE_SIZE];
float local_feature_1[TILE_SIZE * NFEATURES];
int local_membership_2[TILE_SIZE];
float local_feature_2[TILE_SIZE * NFEATURES];
float local_clusters[NCLUSTERS * NFEATURES];
for (int i=0; i<NUM_TILES+2; ++i)
    {
        int load_flag = (i >= 0) && (i < NUM_TILES);
        int compute_flag = (i >= 1) && (i < NUM_TILES+1);
        int store_flag = (i >= 2) && (i < NUM_TILES+2);

        if (i % 3 == 0){
	    	load_local_feature(load_flag, local_feature_0, feature, i);
	    	compute_local_membership(compute_flag, local_feature_2, local_clusters, local_membership_2);
	        store_local_membership(store_flag, local_membership_1, membership, i-2);
        }
        else if (i % 3 == 1){
	    	load_local_feature(load_flag, local_feature_1, feature, i);
	    	compute_local_membership(compute_flag, local_feature_0, local_clusters, local_membership_0);
	        store_local_membership(store_flag, local_membership_2, membership, i-2);
        }
        else{
	    	load_local_feature(load_flag, local_feature_2, feature, i);
	    	compute_local_membership(compute_flag, local_feature_1, local_clusters, local_membership_1);
	        store_local_membership(store_flag, local_membership_0, membership, i-2);
        }


    }
