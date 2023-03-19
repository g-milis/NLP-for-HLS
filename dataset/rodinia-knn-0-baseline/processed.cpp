for(int i = 0; i < NUM_PT_IN_SEARCHSPACE; ++i){
        sum = 0.0;
L2:        for(int j = 0; j < NUM_FEATURE; ++j){
            feature_delta = searchSpace[i*NUM_FEATURE+j] - inputQuery[j];
            sum += feature_delta*feature_delta;
        }
        distance[i] = sum;
    }
