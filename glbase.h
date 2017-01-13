#pragma once

#include <OpenGL/gl.h>

#include "vec.h"
#include "types.h"

void draw_triangle(const vec3 *a, const vec3 *b, const vec3 *c) {
  glBegin(GL_LINE_LOOP);
  glVertex3fv((f32*) a);
  glVertex3fv((f32*) b);
  glVertex3fv((f32*) c);
  glEnd();
}

void draw_line(const vec3 *a, const vec3 *b) {
  glBegin(GL_LINE_LOOP);
  glVertex3fv((f32*) a);
  glVertex3fv((f32*) b);
  glEnd();
}
