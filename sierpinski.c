#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include <stdlib.h>

#include "glbase.h"
#include "glsetup.h"
#include "macro.h"
#include "prng.h"
#include "types.h"
#include "vec.h"

const struct glinit gl_screen_cfg = (struct glinit) {
  .screen_left   = -5,
  .screen_right  = 55.0,
  .screen_bottom = -5,
  .screen_top    = 55.0,
  .screen_near   = -1,
  .screen_far    = +1,
};

const vec3 triangle[3] = {
  {0.0, 0.0, 0.0},    // bottom left corner
  {25.0, 50.0, 0.0},  // top middle
  {50.0, 0.0, 0.0},   // bottom right corner
};
const vec3 p0 = {7.5, 5.0, 0.0};

void sierpinski(int n, vec3 v0, xorshift64s r) {
  glBegin(GL_POINTS);
  for (int i = 0; i < n; i++) {
    int next = prng_next(&r) % _array_len(triangle);
    vec3_middle(&v0, triangle + next, &v0);
    glVertex3fv((f32*)&v0);
  }
  glEnd();
}

void sierpinski2(int n, const vec3 *a, const vec3 *b, const vec3 *c) {
  if (!n) {
    return;
  }

  vec3 x, y, z;
  vec3_middle(a, b, &x);
  vec3_middle(b, c, &y);
  vec3_middle(c, a, &z);
  draw_triangle(&x,&y,&z);

  n--;
  sierpinski2(n, a, &x, &z);
  sierpinski2(n, b, &x, &y);
  sierpinski2(n, c, &y, &z);
}

void draw_sierpinski() {
  glClear(GL_COLOR_BUFFER_BIT);
  xorshift64s r1 = 42;
  int iter = 5000;
  glColor3fv((f32*)&blue);
  draw_triangle_v(triangle, sizeof(*triangle));
  glColor3fv((f32*)&red);
  //sierpinski(iter, p0, r1);
  sierpinski2(7, triangle, triangle + 1, triangle + 2);
  glFlush();
}

int main(int n, char ** args) {
  struct glconfig cfg = (struct glconfig) {
    .n      = n,
    .args   = args,
    .w      = 512,
    .h      = 512,
    .x      = 0,
    .y      = 0,
    .title  = "hello world",
  };
  gl_setup(&cfg);
  gl_init(&gl_screen_cfg);
  gl_display(draw_sierpinski);
  gl_loop();
}
