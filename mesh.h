#pragma once

#include <stdio.h>
#include <assert.h>

#include "macro.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "glbase.h"

void* _alloc(int elems, int elem_size, int *size_var) {
  *size_var = elems;
  void* ptr = malloc(elems * elem_size);
  assert(ptr);
P2("_alloc %p of %d (stored %p)", ptr, elems * elem_size, size_var);
  return ptr;
}

void* _realloc(void *ptr, int elem_size, int *size_var) {
  *size_var = *size_var * 2;
  void* new_ptr = realloc(ptr, *size_var * elem_size);
  assert(new_ptr);
P2("_alloc %p -> %p of %d (stored %p)", ptr, new_ptr, *size_var, size_var);
  return new_ptr;
}

struct Mesh {
  vec3 *points;
  int *index_pairs;
  int n_points;
  int n_points_max;
  int n_pairs;
  int n_pairs_max;
  int points_stride;
};

typedef struct Mesh mesh;

void mesh_draw(mesh *m) {
  // debugging
  for (int i = 0; i < 2*m->n_pairs; i++) {
    int *ptr = m->index_pairs + i;
    P2("%p %d", ptr, *ptr);
  }
  //if (1) return;

  int *idx = m->index_pairs;
  int *end = m->index_pairs + 2 * m->n_pairs;
  while (idx < end) {
    int idx_a = *idx++;
    int idx_b = *idx++;
P2("%d, %d", idx_a, idx_b);
    vec3 *a = m->points + idx_a * m->points_stride;
    vec3 *b = m->points + idx_b * m->points_stride;
    draw_line(a, b);
  }
}

int mesh_add_point(mesh *m, const vec3 *v) {
  if (!m->points) {
    int stride = sizeof(*m->points);
    m->points_stride = stride;
    m->points = _alloc(10, stride, &m->n_points_max);
  }
  if (m->n_points == m->n_points_max) {
    m->points = _realloc(m->points, m->points_stride, &m->n_points_max);
  }
P2("%d", m->n_points);
  int idx = m->n_points++;
  *(m->points + m->points_stride * idx) = *v;
   return idx;
}

void mesh_add_pair(mesh* m, int idx_a, int idx_b) {
  int stride = 2 * sizeof(*m->index_pairs);
  if (!m->index_pairs) {
    m->index_pairs = _alloc(10, stride, &m->n_pairs_max);
  }
  if (m->n_pairs == m->n_pairs_max) {
    m->points = _realloc(m->index_pairs, stride, &m->n_pairs_max);
  }
P2("%d", m->n_pairs);
  int idx = 2 * m->n_pairs++;
  *(m->index_pairs + idx)     = idx_a;
  *(m->index_pairs + idx + 1) = idx_b;
}
