for (i=0; i<SIZE; i++){
L2:		     for (j=i; j<SIZE; j++){
		         sum=result[i*SIZE+j];
L3:			 for (k=0; k<SIZE ; k++) { if(k==i) break; sum -= result[i*SIZE+k]*result[k*SIZE+j]; }
			 result[i*SIZE+j]=sum;
		     }

L4:		     for (j=i+1;j<SIZE; j++){
		         sum=result[j*SIZE+i];
L5:			 for (k=0; k<SIZE ; k++) { if(k==i) break; sum -= result[j*SIZE+k]*result[k*SIZE+i]; }
		         result[j*SIZE+i]=sum/result[i*SIZE+i];
		     }
		 }
