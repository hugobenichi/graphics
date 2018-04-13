#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// TODO: use types.h
// TODO: use macro.h

typedef unsigned char u8;

#define _addr_shift(ptr, offset) (void*)( (u8*)(ptr) + offset )

// utilities for debugging malloc/free
//
// 1) declare a debug_malloc function that takes line and file info
// 2) use a macro to repoint malloc to debug_malloc with __LINE__ and __FILE__
// 3) trace calls to debug_malloc, debug_free and collect somewhere
// 4) regroup by pointer value and now it is trivial to do:
//      - leak analysis
//      - allocation profilling
//
// 5) also: - overallocate everythign and add a magic number at the end
//          - when freeing, scan memory and say if corrupted.

struct mem_debug_tracer {
    void (*tracer_malloc)(void* ptr, size_t size, const char* file, int line);
    void (*tracer_free)(void* ptr, size_t size, long header_tag, long footer_tag, const char* file, int line);
    char data[128];
};

struct mem_debug_tracer mem_debug_tracer = (struct mem_debug_tracer) {};

const /* i64 */ long mem_debug_header_tag = 0xc64ebabe8badc0de;
const /* i64 */ long mem_debug_footer_tag = 0x1234567812345678;

const size_t mem_debug_header_size = sizeof(mem_debug_header_tag) * 2;
const size_t mem_debug_footer_size = sizeof(mem_debug_footer_tag);

void* (*true_malloc)(size_t) = malloc;
void (*true_free)(void*) = free;

void* mem_debug_malloc(size_t size, const char* file, int line) {
  size_t total = size + mem_debug_header_size + mem_debug_footer_size;
  long* p = true_malloc(total);
  if (p) {
    *p = mem_debug_header_tag;
    *(p + 1) = size;
    *(long*)_addr_shift(p, mem_debug_header_size + size) = mem_debug_footer_tag;
  }
  if (mem_debug_tracer.tracer_malloc) {
    mem_debug_tracer.tracer_malloc(p, size, file, line);
  }
  return _addr_shift(p, mem_debug_header_size);
}

void mem_debug_free(void* ptr, const char* file, int line) {
  long* base = _addr_shift(ptr, -mem_debug_header_size);
  long header = *base;
  long size = *(base + 1);
  long footer = *(long*)_addr_shift(ptr, size);
  if (mem_debug_tracer.tracer_free) {
    mem_debug_tracer.tracer_free(base, size, header, footer, file, line);
  }
  true_free(base);
}

void mem_debug_tracer_default_malloc(void* ptr, size_t size, const char* file, int line) {
  FILE* f = *((FILE**) &mem_debug_tracer.data);
  fprintf(f, "%p malloc %s:%d %ld \n", ptr, file, line, size);
}

void mem_debug_tracer_default_free(void* ptr, size_t size, long header_tag, long footer_tag, const char* file, int line) {
  FILE* f = *((FILE**) &mem_debug_tracer.data);
  fprintf(f, "%p free   %s:%d %ld %ld %ld\n", ptr, file, line, size, header_tag, footer_tag);
}

void mem_debug_tracer_set_default(FILE* f) {
  mem_debug_tracer = (struct mem_debug_tracer) {
      .tracer_malloc = mem_debug_tracer_default_malloc,
      .tracer_free = mem_debug_tracer_default_free,
  };
  *((FILE**) &mem_debug_tracer.data) = f;
}

void mem_debug_tracer_clear() {
  mem_debug_tracer = (struct mem_debug_tracer) {};
}

#define malloc(s) mem_debug_malloc(s, __FILE__, __LINE__)
#define free(p) mem_debug_free(p, __FILE__, __LINE__)

// note for struct offset
//
// struct MyStruct {
//   int a
//   float b
//   long c
// };
//
// u32 b_offset = (u32)(((struct MyStruct*) 0) -> b);


struct mem_pool {
  size_t elem_size;
  int elem_nums;
  int elem_used;
  char *mem_block;
  int indices[];
};

struct mem_pool *mem_pool_init(size_t elem_size, int elem_nums) {
  struct mem_pool *p;
  size_t indices_len = elem_nums * sizeof(*p->indices);
  size_t elems_len = elem_size * elem_nums;
  size_t need = sizeof(*p) + indices_len + elems_len;
  p = malloc(need);
  if (p) {
    p->elem_size = elem_size;
    p->elem_nums = elem_nums;
    p->elem_used = 0;
    p->mem_block = ((char*) p) + sizeof(*p) + indices_len;
    for (int i = 0; i < elem_nums; i++) {
      *(p->indices + i) = i;  // set up indices
    }
  }
  return p;
}

void* mem_pool_get(struct mem_pool *p) {
  int next = *(p->indices + p->elem_used);
  p->elem_used++;
  return p->mem_block + p->elem_size * next;
}

void mem_pool_release(struct mem_pool *p, void* release) {
  p->elem_used--;
  // retrieve indice from difference to base pointer
  size_t diff = ((char*)release) - p->mem_block;
  assert(!(diff % p->elem_size));
  *(p->indices + p->elem_used) = diff/p->elem_size;
}

void test_debug_malloc() {
  char* buf1 = malloc(10);
  char* buf2 = malloc(20);
  char* buf3 = malloc(30);
  free(buf1);
  free(buf2);
  char* buf4 = malloc(40);
  char* buf5 = malloc(50);
  free(buf3);
  free(buf4);
  free(buf5);
}

void test_mem_pool() {
  struct mem_pool *p = mem_pool_init(4, 10);

  int *objects[10] = {};

  puts("loop 1");
  for (int i = 0; i < 10; i++) {
      objects[i] = mem_pool_get(p);
  }
  for (int i = 0; i < 10; i++) {
      printf("%d:%p\n", i, objects[i]);
      mem_pool_release(p, objects[i]);
  }

  puts("loop 2");
  for (int i = 0; i < 10; i++) {
      printf("%d:%p\n", i, mem_pool_get(p));
  }

  free(p);
}

int main(int argc, char** args) {
  mem_debug_tracer_set_default(stderr);

  //test_debug_malloc();
  test_mem_pool();

  mem_debug_tracer_clear();
}
