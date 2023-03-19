for (int i = 1; i < N - 1; ++i) {

    const auto right = memory_in[i + 1];

    constexpr float factor = 0.3333;
    const auto average = factor * (left + center + right);

        left = center;
    center = right;

    memory_out[i] = average;
  }
