#include "glsetup.h"
#include "macro.h"
#include "mesh.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "glbase.h"


struct tree_recursion {
  vec3 origin;
  vec3 direction;
  vec3 offset;
  int iter;
  f32 len;
  xorshift64s *r;
  void (*emit)(void *args, const vec3 *a, const vec3 *b);
  void *emit_args;
};

static const vec3 offset      = {1, 0, 0};
static const vec3 offset_neg  = {-1, 0, 0};

void tree_displace(xorshift64s *r, const vec3 *offset, const vec3 *direction, vec3 *out) {
  vec3_mult(offset, 0.2 + 0.5 * prng_next_f64(r), out);
  vec3_add(direction, out, out);
  vec3_renorm(out, out);
}

void tree_recursion_do(struct tree_recursion *tr) {
  if (!tr->iter) return;
  if (prng_next(tr->r) % 5 > tr->iter) return;

  f32 len = tr->len * (0.4 + 0.5 * prng_next_f64(tr->r));

  vec3 end;
  vec3_mult(&tr->direction, len, &end);
  vec3_add(&tr->origin, &end, &end);

  tr->emit(tr->emit_args, &tr->origin, &end);

  vec3 new_direction;
  vec3 new_offset;
  struct tree_recursion next_tr = *tr;
  next_tr.iter--;
  next_tr.len = len;
  next_tr.origin = end;

  tree_displace(tr->r, &tr->offset, &tr->direction, &new_direction);
  next_tr.direction = new_direction;
  tree_recursion_do(&next_tr);

  vec3_mult(&tr->offset, -1, &new_offset);
  tree_displace(tr->r, &new_offset, &tr->direction, &new_direction);
  next_tr.direction = new_direction;
  tree_recursion_do(&next_tr);
}

void draw_tree_rec(int n, const vec3 *origin, const vec3 *direction, float len, xorshift64s *r) {
  if (!n) return;
  if (prng_next(r) % 5 > n) return;

  f32 old_len = len;
  len *= (0.4 + 0.5 * prng_next_f64(r));

  vec3 end;
  vec3_mult(direction, len, &end);
  vec3_add(origin, &end, &end);

  draw_line(origin, &end);

  n--;
  vec3 new_direction;

  tree_displace(r, &offset, direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r);

  tree_displace(r, &offset_neg, direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r);
}

void treerec_emit_draw(void *unused, const vec3 *a, const vec3 *b) {
  draw_line(a, b);
}

void treerec_emit_mesh(void *mesh_to_cast, const vec3 *a, const vec3 *b) {
  mesh *m = (mesh*) mesh_to_cast;
  draw_line(a, b);
  int idx_a = mesh_add_point(m, a);
  int idx_b = mesh_add_point(m, b);
  mesh_add_pair(m, idx_a, idx_b);
}

void draw_tree() {
  glClear(GL_COLOR_BUFFER_BIT);

  vec3 origin = {30, .0, .0};
  vec3 direction = {.0, 1, .0};
  int iter = 10;
  f32 len = 20;
  xorshift64s r = 42;

  struct tree_recursion treerec = (struct tree_recursion) {
    .origin = origin,
    .direction = direction,
    .offset = offset,
    .iter = iter,
    .len = len,
    .r = &r,
    .emit = treerec_emit_draw,
  };

  glColor3fv((f32*)&blue);
  tree_recursion_do(&treerec);

  glColor3fv((f32*)&red);
  origin.x = 15.0;
  r = 42;
  draw_tree_rec(iter, &origin, &direction, len, &r);

  glFlush();
}
