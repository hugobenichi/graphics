#include "glsetup.h"
#include "macro.h"
#include "mesh.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "glbase.h"

static const vec3 offset = {1, 0, 0};

void draw_tree_rec(int n, const vec3 *origin, const vec3 *direction, float len, xorshift64s *r) {
  if (!n) return;
  if (prng_next(r) % 10 > n) return;

  len *= (0.4 + 0.5 * prng_next_f64(r));

  vec3 end;
  vec3_mult(direction, len, &end);
  vec3_add(origin, &end, &end);
  glColor3fv((f32*)&blue);

  draw_line(origin, &end);
//    int idx_a = mesh_add_point(m, origin);
//    int idx_b = mesh_add_point(m, &end);
//    mesh_add_pair(m, idx_a, idx_b);

  n--;
  vec3 new_direction;
  vec3 new_offset;

  vec3_mult(&offset, 0.2 + 0.5 * prng_next_f64(r), &new_offset);
  vec3_add(direction, &new_offset, &new_direction);
  vec3_renorm(&new_direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r);

  vec3_mult(&offset, - 0.2 - 0.5 * prng_next_f64(r), &new_offset);
  vec3_add(direction, &new_offset, &new_direction);
  vec3_renorm(&new_direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r);
}

void draw_tree() {
  glClear(GL_COLOR_BUFFER_BIT);
  vec3 origin = {25, .0, .0};
  vec3 direction = {.0, 1, .0};
  f32 len = 20;
  xorshift64s r = 42;
  draw_tree_rec(14, &origin, &direction, len, &r);
  glFlush();
}
