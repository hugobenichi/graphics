#!/bin/bash

target=sierpinski.c
out=out/sierpinski
#target=mesh.c
#out=out/mesh

mkdir -p out/

[ -f out/glsetup.o ] || clang -o out/glsetup.o -c glsetup.c

clear && clang -g -Os -o $out out/glsetup.o $target \
  -I../utilc \
  -framework GLUT \
  -framework OpenGL
