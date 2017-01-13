#include <GLUT/glut.h>

#include "glsetup.h"

void gl_setup(struct glconfig* cfg) {
  glutInit(&cfg->n, cfg->args);
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
