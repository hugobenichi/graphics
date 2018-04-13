#pragma once

#include <math.h>

#include "macro.h"
#include "types.h"

// f32 vectors
typedef struct Tuple2_Of_F32 v2;
typedef struct Tuple3_Of_F32 vec3;
typedef struct Tuple4_Of_F32 vec4;

// f64 vectors
typedef struct Tuple2_Of_F64 v2d;
typedef struct Tuple3_Of_F64 v3d;
typedef struct Tuple4_Of_F64 v4d;

void vec3_sprint(char *buf, const vec3 *v) {
  sprintf(buf, "{x=%f, y=%f, z=%f}", v->x, v->y, v->z);
}

void vec3_print(const vec3 *v) {
  P("{x=%f, y=%f, z=%f}\n", v->x, v->y, v->z);
}

void vec3_add(const vec3 *in_a, const vec3 *in_b, vec3 *out) {
  out->x = (in_a->x + in_b->x);
  out->y = (in_a->y + in_b->y);
  out->z = (in_a->z + in_b->z);
}

void vec3_sub(const vec3 *in_a, const vec3 *in_b, vec3 *out) {
  out->x = (in_a->x - in_b->x);
  out->y = (in_a->y - in_b->y);
  out->z = (in_a->z - in_b->z);
}

void vec3_neg(const vec3 *in, vec3 *out) {
  out->x = -in->x;
  out->y = -in->y;
  out->z = -in->z;
}

void vec3_mult(const vec3 *in, f32 f, vec3 *out) {
  out->x = f * in->x;
  out->y = f * in->y;
  out->z = f * in->z;
}

void vec3_middle(const vec3 *in_a, const vec3 *in_b, vec3 *out) {
  vec3_add(in_a, in_b, out);
  vec3_mult(out, 0.5, out);
}

f32 vec3_dot(const vec3 *in_a, const vec3 *in_b) {
  return (in_a->x * in_b->x) + (in_a->y * in_b->y) + (in_a->z * in_b->z);
}

f32 vec3_norm2(const vec3 *in) {
  return vec3_dot(in, in);
}

void vec3_renorm(const vec3 *in, vec3 *out) {
  f32 invn = 1.0 / sqrt(vec3_norm2(in));
  vec3_mult(in, invn, out);
}

void vec3_rot90(const vec3 *in, vec3 *out) {
  f32 x = in->x;
  out->x = - in->y;
  out->y = x;
}

void vec3_rot(const vec3 *in, f32 teth, vec3 *out) {
  f32 c = cos(teth);
  f32 s = sin(teth);
  f32 x = in->x;
  f32 y = in->y;
  out->x = c * x - s * y;
  out->y = c * y + s * x;
}
// TODO: normalize, rotate


// ---- vec4 ----

void vec4_add(const vec4 *in_a, const vec4 *in_b, vec4 *out) {
  out->x = (in_a->x + in_b->x);
  out->y = (in_a->y + in_b->y);
  out->z = (in_a->z + in_b->z);
  out->t = (in_a->t + in_b->t);
}

void vec4_mul(const vec4 *in_a, const vec4 *in_b, vec4 *out) {
  out->x = (in_a->x * in_b->x);
  out->y = (in_a->y * in_b->y);
  out->z = (in_a->z * in_b->z);
  out->t = (in_a->t * in_b->t);
}

f32 vec4_dot(const vec4 *in_a, const vec4 *in_b) {
  return (in_a->x * in_b->x)
       + (in_a->y * in_b->y)
       + (in_a->z * in_b->z)
       + (in_a->t * in_b->t);
}

f32 vec4_norm2(const vec4 *a) {
  return vec4_dot(a, a);
}
