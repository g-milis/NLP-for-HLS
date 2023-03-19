




void needwun(char SEQA[ALEN], char SEQB[BLEN],
             char alignedA[ALEN+BLEN], char alignedB[ALEN+BLEN]){

        char ptr[(ALEN+1)*(BLEN+1)];

    int M_former[ALEN+1];
    int M_latter[ALEN+1];

    int score, up_left, up, left, max;
    int row, row_up, r;
    int a_idx, b_idx;
    int a_str_idx, b_str_idx;

                            
        for (a_idx=0; a_idx<ALEN+1; a_idx++) {
        M_former[a_idx] = a_idx*GAP_SCORE;
        ptr[a_idx] = SKIPB;
    }

        fill_out: for(b_idx=1; b_idx<(BLEN+1); b_idx++){
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

        a_idx = ALEN;
    b_idx = BLEN;
    a_str_idx = 0;
    b_str_idx = 0;

    trace: while(a_idx>0 || b_idx>0) {
        r = b_idx*(ALEN+1);
        if (ptr[r + a_idx] == ALIGN){
            alignedA[a_str_idx++] = SEQA[a_idx-1];
            alignedB[b_str_idx++] = SEQB[b_idx-1];
            a_idx--;
            b_idx--;
        }
        else if (ptr[r + a_idx] == SKIPB){
            alignedA[a_str_idx++] = SEQA[a_idx-1];
            alignedB[b_str_idx++] = '-';
            a_idx--;
        }
        else{             alignedA[a_str_idx++] = '-';
            alignedB[b_str_idx++] = SEQB[b_idx-1];
            b_idx--;
        }
    }

        pad_a: for( ; a_str_idx<ALEN+BLEN; a_str_idx++ ) {
      alignedA[a_str_idx] = '_';
    }
    pad_b: for( ; b_str_idx<ALEN+BLEN; b_str_idx++ ) {
      alignedB[b_str_idx] = '_';
    }
}

void needwun_tiling(char* SEQA, char* SEQB,
             char* alignedA, char* alignedB) {
	for (int i=0; i<JOBS_PER_PE; i++) {
	    needwun(SEQA + i*ALEN, SEQB + i*BLEN,
		    alignedA + i*(ALEN+BLEN), alignedB + i*(ALEN+BLEN));
	}
	return;
}

extern "C" {
void workload(char* SEQA, char* SEQB,
             char* alignedA, char* alignedB, int num_jobs) {

	char seqA_buf[UNROLL_FACTOR][ALEN * JOBS_PER_PE];
		char seqB_buf[UNROLL_FACTOR][BLEN * JOBS_PER_PE];
		char alignedA_buf[UNROLL_FACTOR][(ALEN+BLEN) * JOBS_PER_PE];
		char alignedB_buf[UNROLL_FACTOR][(ALEN+BLEN) * JOBS_PER_PE];
	
	int num_batches = num_jobs / JOBS_PER_BATCH;

	int i, j, k;
	major_loop: for (i=0; i<num_batches; i++) {
	    	    reshape1_a: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(seqA_buf[j], SEQA + i*(ALEN*JOBS_PER_BATCH) + j*(ALEN*JOBS_PER_PE), ALEN*JOBS_PER_PE);
	    }
	    reshape1_b: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(seqB_buf[j], SEQB + i*(BLEN*JOBS_PER_BATCH) + j*(BLEN*JOBS_PER_PE), BLEN*JOBS_PER_PE);
	    }
	    	    unroll_loop: for (j=0; j<UNROLL_FACTOR; j++) {
	    		needwun_tiling(seqA_buf[j], seqB_buf[j], alignedA_buf[j], alignedB_buf[j]);
	    }
	    	    reshape2_a: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(alignedA + i*((ALEN+BLEN)*JOBS_PER_BATCH) + j*(ALEN+BLEN)*JOBS_PER_PE, alignedA_buf[j], (ALEN+BLEN)*JOBS_PER_PE);
	    }
	    reshape2_b: for (j=0; j<UNROLL_FACTOR; j++) {
	        memcpy(alignedB + i*((ALEN+BLEN)*JOBS_PER_BATCH) + j*(ALEN+BLEN)*JOBS_PER_PE, alignedB_buf[j], (ALEN+BLEN)*JOBS_PER_PE);
	    }
	}
	return;
}
}
