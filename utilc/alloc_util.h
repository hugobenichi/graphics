#pragma once

#include <stdlib.h>
#include <assert.h>

#include "macro.h"

#define alloc_default_size 10
#define alloc_default_growth 2

#define oversize 1//000

void* debugmalloc(size_t want) {
  void *ptr = malloc(oversize * want);
printf("malloc(%ld) -> %p\n", want, ptr);
  return ptr;
}

void* debugrealloc(void *p, size_t want) {
  void *ptr = realloc(p, oversize * want);
printf("malloc(%p, %ld) -> %p\n", p, want, ptr);
  return ptr;
}

//#define malloc(s) debugmalloc(s)
//#define realloc(p, s) debugrealloc(p, s)

void alloc_new(void **ptr, int *elems, int want_elem, size_t elem_size) {
//P2("%p:%p %p:%d %d %ld", ptr, *ptr, elems, *elems, want_elem, elem_size);
  *elems = want_elem;
  *ptr = malloc(want_elem * elem_size);
  assert(*ptr);
}

void alloc_grow(void **ptr, int *elems, size_t elem_size) {
  int new_elems = *elems * alloc_default_growth;
  *elems = new_elems;
  *ptr = realloc(*ptr, new_elems * elem_size);
  assert(*ptr);
}

#define alloc_ensure_new(ptr, elems, elem_size) if (!ptr) alloc_new((void**)&(ptr), &(elems), alloc_default_size, elem_size)
#define alloc_ensure_grow(ptr, elems, used, elem_size) if (used == elems) alloc_grow((void**)&(ptr), &(elems), elem_size)

#undef malloc
#undef realloc
