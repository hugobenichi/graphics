#!/bin/bash

set -eux

clear

mkdir -p out/

[ -f out/glsetup.o ] || clang -g -o out/glsetup.o -c glsetup.c -Iutilc

clang  -Iutilc -g -Os -o out/mesh out/glsetup.o mesh.c -framework GLUT -framework OpenGL
clang  -Iutilc -g -Os -o out/sierpinski out/glsetup.o sierpinski.c -framework GLUT -framework OpenGL
clang  -Iutilc -g -Os -o out/noise out/glsetup.o noise.c -framework GLUT -framework OpenGL
