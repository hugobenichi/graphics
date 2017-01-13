#pragma once

#include <stdio.h>
#include <assert.h>

#include "macro.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "glbase.h"

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
  int *idx = m->index_pairs;
  int *end = m->index_pairs + 2 * sizeof(*idx) * m->n_points;
  while (idx < end) {
    int idx_a = *idx++;
    int idx_b = *idx++;
    vec3 *a = m->points + idx_a * m->points_stride;
    vec3 *b = m->points + idx_b * m->points_stride;
    draw_line(a, b);
  }
}

int mesh_add_point(mesh *m, const vec3 *v) {
  P2("%p %p %d", m, m->points, m->n_points);
  if (!m->points) {
    P("alloc points %d", m->n_pairs);
    m->points        = malloc(sizeof(*m->points) * 10);
    m->points_stride = sizeof(*m->points);
    m->n_points_max  = 10;
    assert(m->points);
  }
  if (m->n_points == m->n_points_max) {
    P("realloc points %d", m->n_pairs);
    m->n_points_max *= 1.4;
    m->points = realloc(m->points, m->points_stride * m->n_points_max);
    assert(m->points);
  }
  int idx = m->points_stride * m->n_points;
  *(m->points + idx) = *v;
   m->n_points++;
   return idx;
}

void mesh_add_pair(mesh* m, int idx_a, int idx_b) {
  P2("%p %d", m->index_pairs, m->n_pairs);
  if (!m->index_pairs) {
    P("alloc pairs %d", m->n_pairs);
    m->n_pairs_max = 10;
    m->index_pairs = malloc(2 * sizeof(*m->index_pairs) * m->n_pairs_max);
    assert(m->index_pairs);
  }
  if (m->n_pairs == m->n_pairs_max) {
    P("realloc pairs %d", m->n_pairs);
    m->n_pairs_max *= 2; //1.4;
    m->index_pairs = realloc(m->index_pairs, 2 * sizeof(*m->index_pairs) * m->n_pairs_max);
    assert(m->index_pairs);
  }
  int *idx = m->index_pairs + 2 * sizeof(*m->index_pairs) * m->n_pairs;
  *idx++ = idx_a;
  *idx   = idx_b;
   m->n_pairs++;
}
