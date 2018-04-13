#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

#define err_if(expr) if (expr) return -1



// +++ utilz +++ //
//
union Result {
  int has_err; // no error if >= 0
  struct {
    int err_no;
    int line;
    const char* file;
  } err;
  struct {
    int val;
    int val2;
    long val3;
  } ok;
};

struct Iter {
  void* elem;
  void* end;
  void (*next)(struct Iter*);
  int has_left;
  int data;
};

void array_iter_next(struct Iter* it) {
  it -> elem += it -> data;
  it -> has_left--;
}

void array_iter(void* array, size_t elem_size, size_t elem_nums, struct Iter* it) {
  it -> elem = array;
  it -> end = array + elem_size * elem_nums;
  it -> has_left = elem_nums;
  it -> data = elem_size;
  it -> next = array_iter_next;
}

size_t filesize(FILE* f) {
  err_if(0 != fseek(f, 0, SEEK_END));

  long end = ftell(f);
  err_if(end < 0);

  err_if(0 != fseek(f, 0, SEEK_SET));

  return end;
}





// +++ read files +++ //

int read(const char* name, void* buffer, size_t max) {
  FILE* f = fopen(name, "r");
  err_if(!f);

  size_t wants = filesize(f);
  printf("size of %s is %ld bytes\n", name, wants);
  err_if(wants > max);

  size_t got = fread(buffer, 1, wants, f);
  // eof not set because we read exactly thr right number of bytes
  err_if((got != wants));
  printf("wants: %ld, got: %ld\n", wants, got);

  return got;
}

char* string_overwrite(char* s, char c) {
    while (*s) {
        *s++ = c;
    }
    *s++ = 0;
    return s + 1;
}

int main(int argc, char** args) {
  #define SIZE (1024 * 1024)
  size_t bufsiz = SIZE;
  char buffer[SIZE] = {};
  #undef SIZE

  for (int i = 1; i < argc; i++) {
    // header
    char* s = *(args + i);
    char* h1 = buffer;
    strncpy(h1, s, bufsiz);
    char* h2 = string_overwrite(h1, '=');
    strncpy(h2, s, bufsiz);
    char* b = string_overwrite(h2, '-');
    printf("%s\n%s\n%s\n", h1, s, h2);

    int got = read(s, buffer, bufsiz);
    if (got < 0) {
      perror(0);
      exit(0);
    }
    printf("got %d\n", got);

    //int wrote = fwrite(buffer, 1, got, stdout);
    int wrote = 42;
    P(%d, wrote);
    P("%d %d", wrote, got);
  }
}
