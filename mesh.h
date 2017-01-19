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
    draw_line(a, b);
  }
}

void mesh_draw2(mesh *m, void (*disp)(vec3 *p)) {
  // I need to store the displacement of previous points
  // so I am copying stuff
  size_t s = m->n_points * sizeof(*m->points);
  vec3 *displacements = malloc(s);
  memcpy(displacements, m->points, s);
  int *idx = m->pairs;
  int *end = m->pairs + 2 * m->n_pairs;
  while (idx < end) {
    int idx_a = *idx++;
    int idx_b = *idx++;
    vec3 *base_a = m->points + idx_a;
    vec3 *a = displacements + idx_a;
    vec3 *b = displacements + idx_b;

    vec3 base_disp;
    vec3_sub(a, base_a, &base_disp);

// TODO: relative base displacement not correctly applied
// TODO: I should apply base displacement first, then apply relative disp
    vec3_sub(b, a, b);
    disp(b);
    vec3_add(b, a, b);
    vec3_add(b, &base_disp, b);
    draw_line(a, b);
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
