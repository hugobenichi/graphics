#pragma once

#include <OpenGL/gl.h>

#include "macro.h"
#include "types.h"
#include "vec.h"

const vec3 red = {1.0, 0, 0};
const vec3 blue = {0, 0, 1.0};
const vec3 white = {1.0, 1.0, 1.0};

void draw_triangle(const vec3 *a, const vec3 *b, const vec3 *c) {
  glBegin(GL_LINE_LOOP);
  glVertex3fv((f32*) a);
  glVertex3fv((f32*) b);
  glVertex3fv((f32*) c);
  glEnd();
}

void draw_triangle_v(const vec3 *vs, int stride) {
  draw_triangle(vs, _addr_shift(vs, stride), _addr_shift(vs, 2*stride));
}

void draw_line(const vec3 *a, const vec3 *b) {
  glBegin(GL_LINE_LOOP);
  glVertex3fv((f32*) a);
  glVertex3fv((f32*) b);
  glEnd();
}
