#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"
#include "types.h"
#include "tree.h"

int main(int argc, char **args) {

  vec3 origin = {25, .0, .0};
  vec3 direction = {.0, 1, .0};
  int iter = 5;
  f32 len = 20;
  xorshift64s r = 42;

  mesh m = {0};
  struct tree_recursion treerec = (struct tree_recursion) {
    .origin = origin,
    .direction = direction,
    .offset = offset,
    .iter = iter,
    .len = len,
    .r = &r,
    .emit = treerec_emit_mesh,
    //.emit = treerec_emit_draw,
    .emit_args = &m,
  };

  tree_recursion_do(&treerec);
  mesh_draw(&m);

  free(m.points);
  free(m.pairs);
}
