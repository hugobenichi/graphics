#include <stdlib.h>
#include <stdio.h>

#define _offset(struct_name, field_name) ((int)&(((struct_name *) 0) -> field_name))

struct MyStruct {
  int a;
  float b;
  char c;
  long d;
  int e;
};

void sprint_field_int(char* dst, void* obj, int offset) {
  sprintf(dst, "%d", *(int*)(obj + offset));
}

void puts_i32(int i) {
  printf("%d\n", i);
}

int main(int argc, char** args) {

  int offset_a = _offset(struct MyStruct, a);
  int offset_b = _offset(struct MyStruct, b);
  int offset_c = _offset(struct MyStruct, c);
  int offset_d = _offset(struct MyStruct, d);
  int offset_e = _offset(struct MyStruct, e);
  puts_i32(offset_a);
  puts_i32(offset_b);
  puts_i32(offset_c);
  puts_i32(offset_d);
  puts_i32(offset_e);

  puts("");

  struct MyStruct s = (struct MyStruct) {
    .e = 42,
  };

  char buffer[128];
  sprint_field_int(buffer, &s, offset_e);
  buffer[2] = 0;
  printf("%s\n", buffer);

  //printf("%p\n", offset(struct MyStruct, a));
}
