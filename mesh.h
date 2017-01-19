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

void mesh_draw(mesh *m, void (*disp)(vec3 *p)) {
  int *idx = m->pairs;
  int *end = m->pairs + 2 * m->n_pairs;
  while (idx < end) {
    int idx_a = *idx++;
    int idx_b = *idx++;
    vec3 *a = m->points + idx_a;
    vec3 *b = m->points + idx_b;
    vec3 c;
    vec3_add(a, b, &c);
    disp(&c);
    draw_line(a, &c);
  }
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
