#include <stdio.h>

#include "prng.h"

int main(int n, char ** args) {
  xorshift64s r1 = 42;
  xorshift64s r2 = r1 + 1;

  puts("r1:");
  for (int i = 0; i < 30; i++) {
    u64 x = prng_next(&r1);
    printf("%e   %llu\n", prng_f64_of(x), x);
  }

  puts("\nr2:");

  for (int i = 0; i < 30; i++) {
    u64 x = prng_next(&r2);
    printf("%e   %llu\n", prng_f64_of(x), x);
  }

  r2++;
  for (int i = 0; i < 10; i++) {
    u64 x = prng_next(&r2);
    printf("%llu\n", x % 3);
  }
}
