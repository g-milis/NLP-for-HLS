int M[(ALEN+1)*(BLEN+1)];
char ptr[(ALEN+1)*(BLEN+1)];
for(a_idx=0; a_idx<(ALEN+1); a_idx++){
        M[a_idx] = a_idx * GAP_SCORE;
    }
for(b_idx=0; b_idx<(BLEN+1); b_idx++){
        M[b_idx*(ALEN+1)] = b_idx * GAP_SCORE;
    }
for(b_idx=1; b_idx<(BLEN+1); b_idx++){
L6:        fill_in: for(a_idx=1; a_idx<(ALEN+1); a_idx++){
            if(SEQA[a_idx-1] == SEQB[b_idx-1]){
                score = MATCH_SCORE;
            } else {
                score = MISMATCH_SCORE;
            }

            row_up = (b_idx-1)*(ALEN+1);
            row = (b_idx)*(ALEN+1);

            up_left = M[row_up + (a_idx-1)] + score;
            up      = M[row_up + (a_idx  )] + GAP_SCORE;
            left    = M[row    + (a_idx-1)] + GAP_SCORE;

            max = MAX(up_left, MAX(up, left));

            M[row + a_idx] = max;
            if(max == left){
                ptr[row + a_idx] = SKIPB;
            } else if(max == up){
                ptr[row + a_idx] = SKIPA;
            } else{
                ptr[row + a_idx] = ALIGN;
            }
        }
    }
for( ; a_str_idx<ALEN+BLEN; a_str_idx++ ) {
      alignedA[a_str_idx] = '_';
    }
for( ; b_str_idx<ALEN+BLEN; b_str_idx++ ) {
      alignedB[b_str_idx] = '_';
    }
char seqA_buf[ALEN * JOBS_PER_BATCH];
char seqB_buf[BLEN * JOBS_PER_BATCH];
char alignedA_buf[(ALEN+BLEN) * JOBS_PER_BATCH];
char alignedB_buf[(ALEN+BLEN) * JOBS_PER_BATCH];
for (i=0; i<100 ; i++) {
	    	    memcpy(seqA_buf, SEQA + i*(ALEN*JOBS_PER_BATCH), ALEN*JOBS_PER_BATCH);
	    memcpy(seqB_buf, SEQB + i*(BLEN*JOBS_PER_BATCH), BLEN*JOBS_PER_BATCH);
	    L14:	    for (j=0; j<JOBS_PER_BATCH; j++) {
	        needwun(seqA_buf + j*ALEN, seqB_buf + j*BLEN,
			alignedA_buf + j*(ALEN+BLEN), alignedB_buf + j*(ALEN+BLEN));
	    }
	    	    memcpy(alignedA + i*((ALEN+BLEN)*JOBS_PER_BATCH), alignedA_buf, (ALEN+BLEN)*JOBS_PER_BATCH);
	    memcpy(alignedB + i*((ALEN+BLEN)*JOBS_PER_BATCH), alignedB_buf, (ALEN+BLEN)*JOBS_PER_BATCH);
	}
