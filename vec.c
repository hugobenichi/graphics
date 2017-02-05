#include "vec.h"

int main(int argc, char **args) {
  vec4 a, b, c;
  vec4_add(&a, &b, &a);
  vec4_add(&a, &b, &c);
}
