#include <stddef.h>

struct Mem {
  void* mem;
  size_t size;
}

static struct Mem mem_alloc(size_t s) {
  struct Mem m = (struct Mem) {
    .mem  = malloc(s),
    .size = s,
  }
  if (!m.mem) m.mem = 0;
  return m;
}

static void mem_free(struct Mem* m) {
  free(m.mem);
}

// break into Bounded pool (Pool1) and Growing pool (Pool2)

// TODO: parametrize by passing in malloc, calloc, realloc, free
struct Pool {
  struct Mem next;
  size_t  capacity;
  size_t  used;
  struct Mem* mems;
  int mem_num;
};


// parametrized versions
//static int mempool_new(struct Pool *p, size_t size, void *(alloc)(size_t)) {
//static void mempool_free(struct Pool *p, void (memfree)()) {

static int mem_pool_new(struct Pool *p, size_t size) {
  *p = (struct Pool) {
    .next     = mem_alloc(size),
    .capacity = size,
    .used     = 0,
    .mem_num  = 1,
  };

  if (! p -> next.mem) return -1;

  p -> mems = &(p -> next);

  return 0;
}

static void mem_pool_free(struct Pool *p) {
  const int stop = p -> mem_num;
  for (int i = 0; i < stop; i++) {
      mem_free(p -> mems + i);
  }
  if (stop > 1) free(p -> mems);
}

static void* mem_pool_alloc(struct Pool *p, size_t size) {
  // if current Pool1 is full, alloc new Pool1 and continue

  //void* ptr = p -> next;
  //p -> next += size;
  //p -> used += size;
  //return ptr;
  return 0;
}
