#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <nmmintrin.h>
#include <smmintrin.h>

#include "types.h"
#include "macro.h"

// benchmark for 1e6 elems
//  - O2: ~95us
//  - Os: ~95us
//  - xx: 275us
void v2_64_sum(v2_64* out, v2_64* in, int count, int stride) {
  while (count--) {
    out -> x += in -> x;
    out -> y += in -> y;
    in = _addr_shift(in, stride);
  }
}

// TODO does not work for unaligned stride !
void v2_64_sum_sse(v2_64* out, v2_64* in, int count, int stride) {
  __m128 acc = _mm_set_ps(0,0,0,0);
  while (count--) {
    __m128 in_vec = _mm_load_ps((float*)in); // alignement !
    acc = _mm_add_ps(acc, in_vec);
    in = _addr_shift(in, stride);
  }
  _mm_store_ps((float*)out, acc); // alignement ! writing 16B over out !
}

void v2_64_sum_sse_2(v2_64* out, v2_64* in, int count) {
  __m128 acc = _mm_set_ps(0,0,0,0);
  float* fs = (float*) in;
  while (count--) {
    acc = _mm_add_ps(acc, _mm_load_ps(fs));
    fs += 4;
  }
  float temp[4];
  _mm_store_ps(temp, acc); // alignement ! writing 16B over out !
  out -> x = temp[0] + temp[2];
  out -> y = temp[1] + temp[3];
}

i64 timestamp_us() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000L * 1000L + t.tv_usec;
}

int main(int argc, char **args) {
  geom_tests();
  if (1) { return 0; }

  #define count 100000

  v4_128* vecs;
  vecs = malloc(count * sizeof(*vecs));
  if (!vecs) exit(1);

  for (int i = 0; i < count; i++) {
    vecs[i] = (v4_128) {
      .x = 1.0,
      .y = 3.0,
      .z = 5.0,
      .t = 7.0,
    };
  }

  u64 start = timestamp_us();

  v2_64 v[2] = {}; //(v2_64) {0, 0};
  //v2_64_sum(v, (v2_64*) vecs, count, sizeof(*vecs));
  //v2_64_sum_sse(v, (v2_64*) vecs, count, sizeof(*vecs));
  v2_64_sum_sse_2(v, (v2_64*) vecs, count);

  u64 end = timestamp_us();

  printf("(x,y) = (%f,%f)\n", v -> x / count, v -> y / count);
  printf("elapsed %llu us\n", end - start);
}
