#pragma once

#include "glbase.h"
#include "glsetup.h"
#include "macro.h"
#include "mesh.h"
#include "prng.h"
#include "types.h"
#include "vec.h"


const vec3 wind_dir = {1, 0, 0};
const vec3 wind_per = {0, 1, 0};
f32  wind_str = 0.01;

void disp_wind(vec3 *p, vec4* d) {
  //f32 f = wind_str * vec3_dot(&wind_per, p);
  //d->t = -0.05 * fabs(f);
  d->t = - 0.2 * wind_str;
}

struct tree_recursion {
  vec3 origin;
  vec3 direction;
  vec3 offset;
  int iter;
  f32 len;
  xorshift64s *r;
  int (*emit)(struct tree_recursion* self, const vec3 *a, const vec3 *b);
  void *emit_args;
  int last_emit_index;
};

static const vec3 offset      = {1, 0, 0};
static const vec3 offset_neg  = {-1, 0, 0};

void tree_displace(xorshift64s *r, const vec3 *offset, const vec3 *direction, vec3 *out) {
  vec3_mult(offset, 0.2 + 0.5 * prng_next_f64(r), out);
  vec3_add(direction, out, out);
  vec3_renorm(out, out);
}

int tree_emit(struct tree_recursion *tr, const vec3 *a, const vec3 *b) {
  return tr->emit(tr, a, b);
}

void tree_recursion_do(struct tree_recursion *tr) {
  if (!tr->iter) return;
  if (prng_next(tr->r) % 5 > tr->iter) return;

  f32 len = tr->len * (0.4 + 0.5 * prng_next_f64(tr->r));

  vec3 end;
  vec3_mult(&tr->direction, len, &end);
  vec3_add(&tr->origin, &end, &end);

  // If I know origin index, passed in as last_emit_index
  int idx_end = tree_emit(tr, &tr->origin, &end);
  // I get the index of added point

  vec3 new_direction;
  vec3 new_offset;
  struct tree_recursion next_tr = *tr;
  next_tr.iter--;
  next_tr.len = len;
  next_tr.origin = end;
  // I pass down end index to both recursive calls
  next_tr.last_emit_index = idx_end;

  tree_displace(tr->r, &tr->offset, &tr->direction, &new_direction);
  next_tr.direction = new_direction;
  tree_recursion_do(&next_tr);

  vec3_mult(&tr->offset, -1, &new_offset);
  tree_displace(tr->r, &new_offset, &tr->direction, &new_direction);
  next_tr.direction = new_direction;
  tree_recursion_do(&next_tr);
}

int treerec_emit_draw(void *unused, const vec3 *a, const vec3 *b) {
  draw_line(a, b);
  return 0;
}

int treerec_emit_mesh(struct tree_recursion *tr, const vec3 *a, const vec3 *b) {
  mesh *m = (mesh*) tr->emit_args;
  int idx_b = mesh_add_point(m, b);
  mesh_add_pair(m, tr->last_emit_index, idx_b);
  return idx_b;
}

void draw_tree() {
  glClear(GL_COLOR_BUFFER_BIT);

  vec3 origin = {25, .0, .0};
  vec3 direction = {.0, 1, .0};
  int iter = 10;
  f32 len = 20;
  xorshift64s r = 42;

  mesh m = {0};
  int idx_origin = mesh_add_point(&m, &origin);
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
    .last_emit_index = idx_origin,
  };

  glColor3fv((f32*)&blue);
  tree_recursion_do(&treerec);

  mesh_draw(&m, disp_wind);
  glFlush();

  free(m.points);
  free(m.pairs);
}

/*
 * TODO next
 *  try applying deformation function
 *  sync the redraw rate
 */
