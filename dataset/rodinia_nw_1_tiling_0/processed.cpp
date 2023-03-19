int M[(ALEN+1)*(BLEN+1)];
char ptr[(ALEN+1)*(BLEN+1)];
for(a_idx=0; a_idx<(ALEN+1); a_idx++){
        M[a_idx] = a_idx * GAP_SCORE;
    }
for(b_idx=0; b_idx<(BLEN+1); b_idx++){
        M[b_idx*(ALEN+1)] = b_idx * GAP_SCORE;
    }
