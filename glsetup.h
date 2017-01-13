#pragma once

struct glconfig {
  int n;
  char **args;
  int w;
  int h;
  int x;
  int y;
  char *title;
};

void gl_setup(struct glconfig* cfg);
void gl_display(void (*fn)(void));
void gl_loop(void);
