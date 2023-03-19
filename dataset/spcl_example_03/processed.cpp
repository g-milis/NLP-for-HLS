float above_buffer[M];
float center_buffer[M];
for (int i = 0; i < M; ++i) {
    above_buffer[i] = memory_in[i];
  }
for (int i = 0; i < M; ++i) {
    center_buffer[i] = memory_in[M + i];
  }
for (int i = 1; i < N - 1; ++i) {
L6:    for (int j = 0; j < M; ++j) {

      const auto above = above_buffer[j];
      const auto center = center_buffer[j];
      const auto below = memory_in[(i + 1) * M + j];

      constexpr float factor = 0.3333;
      const auto average = factor * (above + center + below);

      memory_out[i * M + j] = average;
    }
  }
