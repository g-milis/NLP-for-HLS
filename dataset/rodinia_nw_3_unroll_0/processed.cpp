for(b_idx=1; b_idx<(BLEN+1); b_idx++){
	    M_latter[0] = M_former[0] + GAP_SCORE;
	    ptr[b_idx*(ALEN+1)] = SKIPA;
        fill_in: for(a_idx=1; a_idx<(ALEN+1); a_idx++){
            if(SEQA[a_idx-1] == SEQB[b_idx-1]){
                score = MATCH_SCORE;
            } else {
                score = MISMATCH_SCORE;
            }

            row = (b_idx)*(ALEN+1);

            up_left = M_former[a_idx-1] + score;
            up      = M_former[a_idx  ] + GAP_SCORE;
            left    = M_latter[a_idx-1] + GAP_SCORE;

            max = MAX(up_left, MAX(up, left));

            M_latter[a_idx] = max;
            if(max == left){
                ptr[row + a_idx] = SKIPB;
            } else if(max == up){
                ptr[row + a_idx] = SKIPA;
            } else{
                ptr[row + a_idx] = ALIGN;
            }
        }

    	for (int k=0; k<ALEN+1; k++) {
    	    M_former[k] = M_latter[k];
    	}
    }
trace: while(a_idx>0 || b_idx>0) {
for( ; a_str_idx<ALEN+BLEN; a_str_idx++ ) {
      alignedA[a_str_idx] = '_';
    }
for( ; b_str_idx<ALEN+BLEN; b_str_idx++ ) {
      alignedB[b_str_idx] = '_';
    }
