for (int i = 0; i < NPOINTS; i++) {
		float min_dist = FLT_MAX;
		int index = 0;

		
L2:		MIN: for (int j = 0; j < NCLUSTERS; j++) {
			float dist = 0.0;

L3:			DIST: for (int k = 0; k < NFEATURES; k++) {
				float diff = feature[NFEATURES * i + k] - clusters[NFEATURES * j + k];
				dist += diff * diff;
			}
			if (dist < min_dist) {
				min_dist = dist;
				index = j;
			}
		}
		
		membership[i] = index;
	}
