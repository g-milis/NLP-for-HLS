for(i = 0; i < BSIZE; i++){
L2:        for (j = 0; j < BSIZE; j++){
            buffer[i * BSIZE + j] = AA(i, j);
        }
    }
for(i = 0; i < BSIZE; i++){
L4:        for (j = 0; j < BSIZE; j++){
            AA(i, j) = buffer[i * BSIZE + j];
        }
    }
float buffer[BSIZE * BSIZE];
for (i = 0; i < BSIZE; i++){
L7:        top:for (j = i; j < BSIZE; j++){
L8:            for (k = 0; k < BSIZE; k++){
                buffer[i * BSIZE + j] = buffer[i * BSIZE + j] - 
                buffer[i * BSIZE + k]* buffer[k * BSIZE + j];
		if(k==i) break;
            }
        }
           
           float temp = 1.f / buffer[i * BSIZE + i];

L9:           left:for (j = i + 1; j < BSIZE; j++){
L10:               for (k = 0; k < BSIZE; k++){
                   buffer[j * BSIZE + i]= buffer[j * BSIZE + i]- buffer[j * BSIZE + k] * buffer[k * BSIZE + i];
               	   if(k==i) break;
	       }
               buffer[j * BSIZE + i] = buffer[j * BSIZE + i] * temp;
           }
    }
for(i = 0; i < BSIZE; i++){
L12:        for(j = 0; j < BSIZE; j++){
            top[i * BSIZE + j] = BB((i_top + i), (j_top + j));
        }
    }
for(i = 0; i < BSIZE; i++){
L14:        for(j = 0; j < BSIZE; j++){
            left[i * BSIZE + j] = BB((i_left + i), (j_left + j));
        }
    }
for(i = 0; i < BSIZE; i++){
L16:        for(j = 0; j < BSIZE; j++){
            BB((i_top + i), (j_top + j)) = top[i * BSIZE + j];
        }
    }
for(i = 0; i < BSIZE; i++){
L18:        for(j = 0; j < BSIZE; j++){
            BB((i_left + i), (j_left + j)) = left[i * BSIZE + j];
        }
    }
float diagonal_buffer[BSIZE * BSIZE];
float top_buffer[BSIZE * BSIZE];
float left_buffer[BSIZE * BSIZE];
for (i = 0; i < BSIZE; i++){
L23:        for (j = 0; j < BSIZE; j++){
            diagonal_buffer[i * BSIZE + j] = AA(i, j);
        }
    }
for (chunk_idx = 0; chunk_idx < 100 ; chunk_idx++){
        perimeter_load(result, top_buffer, left_buffer, offset, chunk_idx);
                
                        float sum; 
                        L25:                        for (j = 0; j < BSIZE; j++){
L26:                            for (i = 0; i < BSIZE; i++){ 
                                sum = 0.0f; 
L27:                                for (k = 0; k < BSIZE; k++){
                                    sum += diagonal_buffer[BSIZE * i + k] * top_buffer[k * BSIZE + j];
                                    if(k==j) break;
				}

                                top_buffer[i * BSIZE + j] = top_buffer[i * BSIZE + j] - sum;
                            }
                        }


                        L28:                        for (i = 0; i < BSIZE; i++){
L29:                            for (j = 0; j < BSIZE; j++){
                                 sum = 0.0f;
L30:                                 for (k = 0; k < BSIZE; k++){
                                     sum += left_buffer[i * BSIZE + k] * diagonal_buffer[BSIZE * k + j];
                                     if(k==j) break;
				 }

                                 left_buffer[i * BSIZE + j] = (left_buffer[i * BSIZE + j] - sum) / diagonal_buffer[j * BSIZE + j];
                            }
                        }

                        perimeter_store(result, top_buffer, left_buffer, offset, chunk_idx);

    }
