#pragma once

#include <stdio.h>
#include <string.h>

#include "types.h"

#define _cat_impl(a,b) a##b
#define _cat(a,b) _cat_impl(a, b)

#define _addr_shift(ptr, offset) (void*)( (u8*)(ptr) + offset )

#define P(format, ...)  printf(#__VA_ARGS__ " = " #format "\n", __VA_ARGS__)
#define P2(format, ...)  printf(__FILE__ ":%04d] " #__VA_ARGS__ " = " #format "\n", __LINE__, __VA_ARGS__)

#define _array_len(ary)   (sizeof(ary) / sizeof(*ary))

void print_mem(void *ptr, int n) {
  u8 *mem = (u8*) ptr;
  char buf[1024];
  size_t left = sizeof(buf);
  char *p = buf;
  for (int i = 0; i < n; i++) {
    int wrote = snprintf(p, left, "0x%02x,", *mem);
    p += wrote;
    left -= wrote;
    mem++;
  }
  *++p = 0;
  puts(buf);
}

void toggle_setting(int *out, int *val1_p, int *val2_p) {
  int val1 = *val1_p;
  int val2 = *val2_p;
  if (*out == val1) {
    *out = val2;
    return;
  }
  *out = val1;
}

void toggle_setting_bool(int *out) {
  *out = !*out;
}
