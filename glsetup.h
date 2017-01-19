#pragma once

#include "types.h"

struct glconfig {
  int n;
  char **args;
  int w;
  int h;
  int x;
  int y;
  char *title;
};

struct glinit {
  f32 screen_left;
  f32 screen_right;
  f32 screen_bottom;
  f32 screen_top;
  f32 screen_near;
  f32 screen_far;
};

void gl_setup(const struct glconfig* cfg);
void gl_display(void (*fn)(void));
void gl_loop(void);
void gl_init(const struct glinit* cfg);
void gl_redisplay(void);
