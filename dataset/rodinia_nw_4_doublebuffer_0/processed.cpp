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
