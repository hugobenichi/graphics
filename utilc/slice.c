#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "macro.h"

// Arrays: DOCME
struct Array {
  int len; // TODO: should be unsigned
  int max;
  void* ptr; // TODO consider tagging the ptr last bit to indicate alignement ?
};

typedef struct Array array;

void array_borrow(array* a, void* ptr, int capacity) {
  a->len = 0;
  a->max = capacity;
  a->ptr = ptr;
}

// array_make allocate one block only, therefore
//   - should ptr go away and memory be appended right after ?
//   - or is there a way to allow both borrowed arrays (non-contiguous ptr) and owned arrays ?
array* array_make(int capacity) {
  array* a;
  size_t total_size = sizeof(*a) + capacity;
  a = malloc(total_size);
  if (a) {
    array_borrow(a, a + sizeof(*a), capacity);
  }
  return a;
}

void array_add_obj(array* a, void* obj, size_t obj_len) {
  size_t need = a->len + obj_len;
  if (need > a->max) {
    int new_max = a->len;
    while (new_max < need) {
      new_max *= 2;
    }
    a->ptr = realloc(a, new_max + sizeof(*a));
    if (a->ptr) {
        return;
    }
    a->max = new_max;
  }
  memcpy(a->ptr + a->len, obj, obj_len);
  a->len += obj_len;
}

#define _instantiate_array_getter(name, type) \
  type* name (array* a, int idx) { \
    type *p = (type*) a->ptr; \
    assert(idx * sizeof(*p) < a->len); \
    return p + idx; \
  }
_instantiate_array_getter(array_get_u8, u8);
_instantiate_array_getter(array_get_u16, u16);
_instantiate_array_getter(array_get_u32, u32);
_instantiate_array_getter(array_get_u64, u64);
#undef _instantiate_array_getter

#define _instantiate_array_last(name, type) \
  type* name (array* a) { \
    return _cat(array_get_, type) (a, a->len-1); \
  }

_instantiate_array_last(array_last_u8, u8);
_instantiate_array_last(array_last_u16, u16);
_instantiate_array_last(array_last_u32, u32);
_instantiate_array_last(array_last_u64, u64);
#undef _instantiate_array_last

// TODO: add_u* or grow. use case: append element

// Slices: DOCME

struct Slice {
  array *a;
  int start;
  int len;
};

typedef struct Slice slice;

#define _slice_borrow(ary_ptr) (slice) { .a = ary_ptr, .start = 0, .len = ary_ptr -> len }

slice slice_make(int capacity) {
  array *a = array_make(capacity);
  return _slice_borrow(a);
}

slice slice_concat(slice s, void* obj, int obj_len) {
  array_add_obj(s.a, obj, obj_len);
  s.len += obj_len;
  return s;
}

void example() {
    const char* bla = "bla";
    int bla_len = strlen(bla);
    int repeat = 10;

    slice s = slice_make(5); //repeat * bla_len + 1);
    s.len = 0;

    for (int i = 0; i < repeat; i++) {
        s = slice_concat(s, (void*) bla, bla_len);
    }
    printf("s.len = %d\n", s.len);

    *array_last_u8(s.a) = 0;
    //*(char *)(s.a->ptr + s.len) = 0;

    printf("%s\n", s.a->ptr);
}

int main(int argc, char** args) {
  example();
}
