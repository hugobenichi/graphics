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

#define immediate 1

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

const vec3 red = {1.0, 0, 0};
const vec3 blue = {0, 0, 1.0};
const vec3 white = {1.0, 1.0, 1.0};
const vec3 p0 = {7.5, 5.0, 0.0};

void init_attributes() {
  glClearColor(1,1,1,1);
  glPointSize(2.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(screen_left, screen_right, screen_bottom, screen_top, screen_near, screen_far);
  glMatrixMode(GL_MODELVIEW);
}

void draw_triangle_v(const vec3 *vs, int stride) {
  draw_triangle(vs, _addr_shift(vs, stride), _addr_shift(vs, 2*stride));
}

void draw_sierpinski(int n, vec3 v0, xorshift64s r) {
  glBegin(GL_POINTS);
  for (int i = 0; i < n; i++) {
    int next = prng_next(&r) % _array_len(triangle);
    vec3_middle(&v0, triangle + next, &v0);
    glVertex3fv((f32*)&v0);
  }
  glEnd();
}

void draw_sierpinski2(int n, vec3 *a, vec3 *b, vec3 *c) {
  if (!n) {
    return;
  }

  vec3 x, y, z;
  vec3_middle(a, b, &x);
  vec3_middle(b, c, &y);
  vec3_middle(c, a, &z);
  draw_triangle(&x,&y,&z);

  n--;
  draw_sierpinski2(n, a, &x, &z);
  draw_sierpinski2(n, b, &x, &y);
  draw_sierpinski2(n, c, &y, &z);
}

void draw_scene1_sierpinski() {
  xorshift64s r1 = 42;
  int iter = 5000;
  glColor3fv((f32*)&blue);
  draw_triangle_v(triangle, sizeof(*triangle));
  glColor3fv((f32*)&red);
  //draw_sierpinski(iter, p0, r1);
  draw_sierpinski2(7, triangle, triangle + 1, triangle + 2);
}

const vec3 offset = {1, 0, 0};

void draw_tree_rec(int n, const vec3 *origin, const vec3 *direction, float len, xorshift64s *r, mesh* m) {
  if (!n) return;
  if (prng_next(r) % 10 > n) return;

  len *= (0.4 + 0.5 * prng_next_f64(r));

  vec3 end;
  vec3_mult(direction, len, &end);
  vec3_add(origin, &end, &end);
  glColor3fv((f32*)&blue);

  int idx_a = mesh_add_point(m, origin);
  int idx_b = mesh_add_point(m, &end);
  mesh_add_pair(m, idx_a, idx_b);
  if (immediate) draw_line(origin, &end);

  n--;
  vec3 new_direction;
  vec3 new_offset;

  vec3_mult(&offset, 0.2 + 0.5 * prng_next_f64(r), &new_offset);
  vec3_add(direction, &new_offset, &new_direction);
  vec3_renorm(&new_direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r, m);

  vec3_mult(&offset, - 0.2 - 0.5 * prng_next_f64(r), &new_offset);
  vec3_add(direction, &new_offset, &new_direction);
  vec3_renorm(&new_direction, &new_direction);
  draw_tree_rec(n, &end, &new_direction, len, r, m);
}

static xorshift64s r = 42;
static mesh *the_tree;

void draw_tree(mesh* m) {
  vec3 origin = {25, .0, .0};
  vec3 direction = {.0, 1, .0};
  f32 len = 20;
  //r++;
  draw_tree_rec(14, &origin, &direction, len, &r, m);
}

void draw_the_tree() {
  mesh_draw(&the_tree);
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT);
  //draw_scene1_sierpinski();
  mesh m = {0, 0, 0, 0, 0, 0, 0};
  draw_tree(&m);
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
  //glutInit(&n, args);
  //glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  //glutInitWindowSize(512,512);
  //glutInitWindowPosition(0,0);
  //glutCreateWindow("hello world");

  init_attributes();

  the_tree = malloc(sizeof(*the_tree));
  memset(the_tree, 0, sizeof(*the_tree));
  print_mem(the_tree, sizeof(*the_tree));

  //draw_tree(the_tree);
  //print_mem(the_tree, sizeof(*the_tree));

  gl_display(draw);
  //glutDisplayFunc(draw);
  //glutDisplayFunc(draw_the_tree);

  //glutMainLoop();
  gl_loop();
}
