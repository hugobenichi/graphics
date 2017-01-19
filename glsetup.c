#include <GLUT/glut.h>
#include <OpenGL/gl.h>

#include "glsetup.h"

void gl_setup(const struct glconfig* cfg) {
  int n = cfg->n; // not const
  glutInit(&n, cfg->args);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(cfg->w, cfg->h);
  glutInitWindowPosition(cfg->x, cfg->y);
  glutCreateWindow(cfg->title);
}

void gl_display(void (*fn)()) {
  glutDisplayFunc(fn);
}

void gl_loop() {
  glutMainLoop();
}

void gl_init(const struct glinit* cfg) {
  glClearColor(1,1,1,1);
  glPointSize(2.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(cfg->screen_left, cfg->screen_right, cfg->screen_bottom, cfg->screen_top, cfg->screen_near, cfg->screen_far);
  glMatrixMode(GL_MODELVIEW);
}
