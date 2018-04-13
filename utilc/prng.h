#pragma once

#include "types.h"

// a reimplementation of http://xorshift.di.unimi.it/
// TODO:
//		implement MT19937-64 ?
//		add jump functions
//    pass them through http://simul.iro.umontreal.ca/testu01/tu01.html
//    add xorshift128+, xorshift1024*

// TODO: consider renaming to prng_t, or prng_t1
typedef u64 xorshift64s;

static const f64 f = 1. / (1L << 52);
static const u64 m = (1L << 52) - 1;

f64 prng_f64_of(u64 x) {
  // TODO optimize this by removing the float multiplicastion
  return f * (f64) (x & m);
}

// TODO: test this
u64 prng_next(xorshift64s* r) {
	u64 u = *r;
	u ^= u >> 12;
	u ^= u << 25;
	u ^= u >> 27;
	*r = u;
	return u * 2685821657736338717;
}

f64 prng_next_f64(xorshift64s* r) {
  return prng_f64_of(prng_next(r));
}
