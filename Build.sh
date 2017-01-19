#!/bin/bash

set -eux

clear

mkdir -p out/

[ -f out/glsetup.o ] || clang -g -o out/glsetup.o -c glsetup.c -I../utilc

clang  -I../utilc -g -Os -o out/mesh out/glsetup.o mesh.c -framework GLUT -framework OpenGL
clang  -I../utilc -g -Os -o out/sierpinski out/glsetup.o sierpinski.c -framework GLUT -framework OpenGL
