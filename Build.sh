#!/bin/bash

#clang -c glsetup.c

clear && clang -g -Os -o sierpinski glsetup.o sierpinski.c \
  -I../utilc \
  -framework GLUT \
  -framework OpenGL
