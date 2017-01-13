#!/bin/bash

mkdir -p out/

[ -f out/glsetup.o ] || clang -o out/glsetup.o -c glsetup.c

clear && clang -g -Os -o out/sierpinski out/glsetup.o sierpinski.c \
  -I../utilc \
  -framework GLUT \
  -framework OpenGL
