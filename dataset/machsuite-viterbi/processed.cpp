prob_t llike[N_OBS][N_STATES];
for( s=0; s<N_STATES; s++ ) {
    llike[0][s] = init[s] + emission[s*N_TOKENS+obs[0]];
  }
for( t=1; t<N_OBS; t++ ) {
L4:    L_curr_state: for( curr=0; curr<N_STATES; curr++ ) {
            prev = 0;
      min_p = llike[t-1][prev] +
              transition[prev*N_STATES+curr] +
              emission[curr*N_TOKENS+obs[t]];
L5:      L_prev_state: for( prev=1; prev<N_STATES; prev++ ) {
        p = llike[t-1][prev] +
            transition[prev*N_STATES+curr] +
            emission[curr*N_TOKENS+obs[t]];
        if( p<min_p ) {
          min_p = p;
        }
      }
      llike[t][curr] = min_p;
    }
  }
for( s=1; s<N_STATES; s++ ) {
    p = llike[N_OBS-1][s];
    if( p<min_p ) {
      min_p = p;
      min_s = s;
    }
  }
for( t=N_OBS-2; t>=0; t-- ) {
    min_s = 0;
    min_p = llike[t][min_s] + transition[min_s*N_STATES+path[t+1]];
L8:    L_state: for( s=1; s<N_STATES; s++ ) {
      p = llike[t][s] + transition[s*N_STATES+path[t+1]];
      if( p<min_p ) {
        min_p = p;
        min_s = s;
      }
    }
    path[t] = min_s;
  }
