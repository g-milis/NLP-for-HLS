for (int i = 0; i < N; ++i) {
    const auto a = a_mem[i];
    const auto b = b_mem[i];
                    float c = (a + b) * (a - b);
        c_mem[i] = c;
  }
