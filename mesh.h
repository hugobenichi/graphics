#pragma once

#include <stdio.h>
#include <assert.h>

#include "glbase.h"
#include "macro.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "alloc_util.h"

struct Mesh {
  vec3 *points;
  int *pairs;       // pairs of indexes into the points array
  int n_points;
  int n_points_max;
  int n_pairs;
  int n_pairs_max;
};

typedef struct Mesh mesh;

void mesh_draw(mesh *m, void (*disp)(vec3 *p, vec4 *d)) {
  vec4 *displacements = calloc(m->n_points, sizeof(vec4));
  int *idx = m->pairs;
  int *end = m->pairs + 2 * m->n_pairs;

  idx += 2;
  draw_line(m->points, (m->points + 1));
  while (idx < end) {
    int idx_a = *idx++;
    int idx_b = *idx++;

    vec3 *a = m->points + idx_a;
    vec3 *b = m->points + idx_b;
    vec4 *disp_a = displacements + idx_a;
    vec4 *disp_b = displacements + idx_b;

// BUG: seems almost good, but the effect does not totally
// aggregate itself correctly for end segments
    // first transform segment b - a into b' - a' by applying
    // disp_a coming from previous segment ending in a.
    vec3 ab, a2, b2;
    vec3_sub(b, a, &ab);
    vec3_add(a, (vec3*) disp_a, &a2); // a' = dp1.translation(a)
    vec3_rot(&ab, disp_a->t, &ab);    // (b' - a') = dp1.rotation(b - a)
    // second compute displacement on b' - a' to get b'' - a'
    //    (ignoring translation part)
    disp(&ab, disp_b);
    vec3_rot(&ab, disp_b->t, &ab);    // b'' - a' = dp2(b' - a')
    vec3_add(&a2, &ab, &b2);          // get b''
    // third write total translation on b: b'' - b
    vec3_sub(&b2, b, (vec3*) disp_b);

//P((%e,%e) -> (%e,%e), a->x, a->y, b->x, b->y);
//P((%e,%e) -> (%e,%e), a2.x, a2.y, b2.x, b2.y);
    //draw_line(a, b);
    draw_line(&a2, &b2);
  }
  free(displacements);
}

int mesh_add_point(mesh *m, const vec3 *v) {
  int stride = sizeof(*m->points);
  alloc_ensure_new(m->points, m->n_points_max, stride);
  alloc_ensure_grow(m->points, m->n_points_max, m->n_points, stride);
  int idx = m->n_points++;
  P2(points addr %p + %d * %d = %p, m->points, stride, idx, m->points + stride * idx);
  *(m->points + idx) = *v;
   return idx;
}

void mesh_add_pair(mesh* m, int idx_a, int idx_b) {
  int stride = 2 * sizeof(*m->pairs);
P2(%p %d %d %d, m->pairs, m->n_pairs_max, m->n_pairs, stride);
  alloc_ensure_new(m->pairs, m->n_pairs_max, stride);
  alloc_ensure_grow(m->pairs, m->n_pairs_max, m->n_pairs, stride);
  int idx = 2 * m->n_pairs++;
  *(m->pairs + idx)     = idx_a;
  *(m->pairs + idx + 1) = idx_b;
}
