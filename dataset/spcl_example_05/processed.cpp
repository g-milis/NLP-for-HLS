for (int n = 0; n < N; ++n) {
L2:    double acc[M];  
L3:    for (int k = 0; k < K; ++k) {  
      const auto a = A[n * K + k];  
L4:      for (int m = 0; m < M; ++m) {
        const double prev = (k == 0) ? 0 : acc[m];          acc[m] = prev + a * B[k * M + m];                         }
    }

    L5:    for (int m = 0; m < M; ++m) {
      C[n * M + m] = acc[m];
    }
  }
