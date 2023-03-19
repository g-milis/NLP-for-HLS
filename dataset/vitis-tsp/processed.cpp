


constexpr long int factorial(const int N)
{
    long int ret = 1;
    for(int i = 0 ; i < N; ++i)
      ret = ret * (i+1);
    return ret;
}



template<typename T>
unsigned int getDistance(const T perm[N], const uint16_t distances[N][N])
{
  unsigned int ret = 0;
  for(int i = 0; i < N-1; ++i)
    ret += distances[perm[i]][perm[i+1]];
  return ret;

}


auto compute(const unsigned long int i_, const uint16_t distances[N][N])
{
  
  unsigned long int i = i_;
  int perm[N] = {0};

              for (int k = 0; k < N; ++k) {
    perm[k] = i / factorial(N - 1 - k);
    i = i % factorial(N - 1 - k);
  }

  for (char k = N - 1; k > 0; --k)
    for (char j = k - 1; j >= 0; --j)
      perm[k] += (perm[j] <= perm[k]);

              
  std::cout << "getDistance: "<< getDistance(perm,distances) << std::endl;

  return getDistance(perm,distances);

}


void tsp(hls::stream<uint16_t>& streamDistances, unsigned int& shortestDistance)
{
        
    uint16_t distances_0[N][N];
    uint16_t distances_1[N][N];
    uint16_t distances_2[N][N];
    uint16_t distances_3[N][N];
                
        loop_distances: for (int i = 0; i < N*N; ++i)
    {
        uint16_t val;
        streamDistances >> val;
        distances_0[i/N][i%N] = val;
        distances_1[i/N][i%N] = val;
        distances_2[i/N][i%N] = val;
        distances_3[i/N][i%N] = val;
    }

        constexpr long int factorialN = factorial(N-1);
    unsigned int candidate0 = std::numeric_limits<unsigned int>::max();
    unsigned int candidate1 = std::numeric_limits<unsigned int>::max();
    unsigned int candidate2 = std::numeric_limits<unsigned int>::max();
    unsigned int candidate3 = std::numeric_limits<unsigned int>::max();
    loop_compute: for( unsigned long int i_ = 0; i_ < factorialN; i_ += 4 )
    {
      candidate0 = std::min(candidate0, compute(i_+0, distances_0));
      candidate1 = std::min(candidate1, compute(i_+1, distances_1));
      candidate2 = std::min(candidate2, compute(i_+2, distances_2));
      candidate3 = std::min(candidate3, compute(i_+3, distances_3));
    }

        shortestDistance = std::min({ candidate0, candidate1,
                                  candidate2, candidate3 });


}
