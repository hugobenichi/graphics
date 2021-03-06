#include <stdio.h>
#include <stdlib.h>

#include "glsetup.h"
#include "mesh.h"
#include "types.h"
#include "tree.h"

const struct glinit gl_screen_cfg = (struct glinit) {
  .screen_left   = -5,
  .screen_right  = 55.0,
  .screen_bottom = -5,
  .screen_top    = 55.0,
  .screen_near   = -1,
  .screen_far    = +1,
};

static mesh the_tree = {0};

int draw_calls = 0;

xorshift64s wind_force_r = 1;

f32 disp_off = 0.01;
void draw_the_tree() {
  P(%d, draw_calls++);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3fv((f32*)&blue);
  mesh_draw(&the_tree, disp_wind);
  glFlush();

  f32 dp = 0;
  for (int i = 0; i < 20; i++) {
    dp += prng_next_f64(&wind_force_r) - 0.5;
  }

  P(%e, dp);
  wind_str += dp/10 + disp_off;
  if (draw_calls % 40 == 0) {
    disp_off = -disp_off;
  }

  gl_redisplay();
}

void make_the_tree() {
  xorshift64s r = 42;
  vec3 origin = {25, .0, .0};
  int idx_origin = mesh_add_point(&the_tree, &origin);
  struct tree_recursion treerec = (struct tree_recursion) {
    .origin     = origin,
    .direction  = {0, 1, 0},
    .offset     = offset,
    .iter       = 10,
    .len        = 20,
    .r          = &r,
    .emit       = treerec_emit_mesh,
    .emit_args  = &the_tree,
    .last_emit_index = idx_origin,
  };
  tree_recursion_do(&treerec);
}

int main(int argc, char **args) {
  make_the_tree();

  struct glconfig cfg = (struct glconfig) {
    .n      = argc,
    .args   = args,
    .w      = 512,
    .h      = 512,
    .x      = 0,
    .y      = 0,
    .title  = "hello world",
  };
  gl_setup(&cfg);
  gl_init(&gl_screen_cfg);

  //mesh_draw(&the_tree, disp_nothing);
  //gl_display(draw_tree);
  gl_display(draw_the_tree);
  gl_loop();

  free(the_tree.points);
  free(the_tree.pairs);
}
