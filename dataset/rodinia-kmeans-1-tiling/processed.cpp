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
int local_membership[TILE_SIZE];
float local_feature[TILE_SIZE * NFEATURES];
float local_clusters[NCLUSTERS * NFEATURES];
for (ii=0; ii<NUM_TILES; ++ii)
    {
    	load_local_feature(local_feature, feature, ii);
    	compute_local_membership(local_feature, local_clusters, local_membership);
        store_local_membership(local_membership, membership, ii);
    }
