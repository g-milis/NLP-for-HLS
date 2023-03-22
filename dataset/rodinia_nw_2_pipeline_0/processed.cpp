char ptr[(ALEN+1)*(BLEN+1)];
former[ALEN+1];
L3:    char M_latter[ALEN+1];

    char score, up_left, up, left, max;
    int row, row_up, r;
    int a_idx, b_idx;
    int a_str_idx, b_str_idx;

L4:    for (a_idx=0; a_idx<ALEN+1; a_idx++) {
        M_former[a_idx] = a_idx*GAP_SCORE;
    }
for (a_idx=0; a_idx<ALEN+1; a_idx++) {
        ptr[a_idx] = SKIPB;
    }
for(b_idx=1; b_idx<(BLEN+1); b_idx++){
L7:        fill_in: for(a_idx=0; a_idx<(ALEN+1); a_idx++){
    	    if (a_idx == 0) {
    	        M_latter[0] = b_idx * GAP_SCORE;
                ptr[b_idx*(ALEN+1)] = SKIPA;
    	    }
    	    else {
                    if(SEQA[a_idx-1] == SEQB[b_idx-1]){
                        score = MATCH_SCORE;
                    } else {
                        score = MISMATCH_SCORE;
                    }

    	        char x = M_former[ALEN];
                    char y = M_former[0  ];
                    char z = M_latter[ALEN];

                    up_left = x + score;
                    up      = y + GAP_SCORE;
                    left    = z + GAP_SCORE;

                    max = MAX(up_left, MAX(up, left));

                    M_latter[0] = max;

                    row = (b_idx)*(ALEN+1);
                    if(max == left){
                        ptr[row + a_idx] = SKIPB;
                    } else if(max == up){
                        ptr[row + a_idx] = SKIPA;
                    } else{
                        ptr[row + a_idx] = ALIGN;
                    }
    	    }
	        	    char tmp_former = M_former[0];
    	    char tmp_latter = M_latter[0];

L8:    	    for(int i=0; i<ALEN+1-1; i++){
    	        M_former[i] = M_former[i+1] ; 
    	        M_latter[i] = M_latter[i+1] ; 
	        }

    	    M_former[ALEN+1-1] = tmp_former;
    	    M_latter[ALEN+1-1] = tmp_latter;
        }

L9:    	for (int k=0; k<ALEN+1; k++) {
    	    M_former[k] = M_latter[k];
    	}
    }
for( ; a_str_idx<ALEN+BLEN; a_str_idx++ ) {
      alignedA[a_str_idx] = '_';
    }
for( ; b_str_idx<ALEN+BLEN; b_str_idx++ ) {
      alignedB[b_str_idx] = '_';
    }
for (int i=0; i<JOBS_PER_PE; i++) {
	    needwun(SEQA + i*ALEN, SEQB + i*BLEN,
		    alignedA + i*(ALEN+BLEN), alignedB + i*(ALEN+BLEN));
	}
char seqA_buf[UNROLL_FACTOR][ALEN * JOBS_PER_PE];
char seqB_buf[UNROLL_FACTOR][BLEN * JOBS_PER_PE];
char alignedA_buf[UNROLL_FACTOR][(ALEN+BLEN) * JOBS_PER_PE];
char alignedB_buf[UNROLL_FACTOR][(ALEN+BLEN) * JOBS_PER_PE];
for (i=0; i<100 ; i++) {
	    L18:	    reshape1_a: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(seqA_buf[j], SEQA + i*(ALEN*JOBS_PER_BATCH) + j*(ALEN*JOBS_PER_PE), ALEN*JOBS_PER_PE);
	    }
L19:	    reshape1_b: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(seqB_buf[j], SEQB + i*(BLEN*JOBS_PER_BATCH) + j*(BLEN*JOBS_PER_PE), BLEN*JOBS_PER_PE);
	    }
	    L20:	    unroll_loop: for (j=0; j<UNROLL_FACTOR; j++) {
            needwun_tiling(seqA_buf[j], seqB_buf[j], alignedA_buf[j], alignedB_buf[j]);
	    }
	    L21:	    reshape2_a: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(alignedA + i*((ALEN+BLEN)*JOBS_PER_BATCH) + j*(ALEN+BLEN)*JOBS_PER_PE, alignedA_buf[j], (ALEN+BLEN)*JOBS_PER_PE);
	    }
L22:	    reshape2_b: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(alignedB + i*((ALEN+BLEN)*JOBS_PER_BATCH) + j*(ALEN+BLEN)*JOBS_PER_PE, alignedB_buf[j], (ALEN+BLEN)*JOBS_PER_PE);
	    }
	}
