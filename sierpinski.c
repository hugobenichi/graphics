#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include <stdlib.h>

#include "glsetup.h"
#include "macro.h"
#include "mesh.h"
#include "prng.h"
#include "types.h"
#include "vec.h"
#include "glbase.h"
#include "tree.h"

const f32 screen_left   = -5;
const f32 screen_right  = 55.0;
const f32 screen_bottom = -5;
const f32 screen_top    = 55.0;
const f32 screen_near   = -1;
const f32 screen_far    = +1;

const vec3 triangle[3] = {
  {0.0, 0.0, 0.0},    // bottom left corner
  {25.0, 50.0, 0.0},  // top middle
  {50.0, 0.0, 0.0},   // bottom right corner
};
const vec3 p0 = {7.5, 5.0, 0.0};

void init_attributes() {
  glClearColor(1,1,1,1);
  glPointSize(2.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(screen_left, screen_right, screen_bottom, screen_top, screen_near, screen_far);
  glMatrixMode(GL_MODELVIEW);
}

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

static mesh *the_tree;

void draw_the_tree() {
  mesh_draw(the_tree);
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

  init_attributes();

  //the_tree = malloc(sizeof(*the_tree));
  //memset(the_tree, 0, sizeof(*the_tree));
  //print_mem(the_tree, sizeof(*the_tree));
  //make_tree(the_tree);
  //print_mem(the_tree, sizeof(*the_tree));

  gl_display(draw_tree);
  //gl_display(draw_the_tree);
  //gl_display(draw_sierpinski);
  gl_loop();
}
