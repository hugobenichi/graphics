#!/bin/bash

clear && clang -g -Os -o sierpinski sierpinski.c \
  -I../utilc \
  -framework GLUT \
  -framework OpenGL
