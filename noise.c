#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "macro.h"
#include "glbase.h"
#include "glsetup.h"
#include "prng.h"
#include "types.h"

struct Texture {
  int rows;
  int columns;
  f32 array[];
};

typedef struct Texture texture;

texture* texture_make(int rows, int columns) {
  texture *t = malloc(rows * columns * sizeof(f32));
  if (t) {
    t->rows = rows;
    t->columns = columns;
  }
  return t;
}

void texture_fill(texture *t, f32 f) {
  int cell_left = t->rows * t->columns;
  f32 *array = t->array;
  while(cell_left--) {
    *array++ = f;
  }
}

void texture_clear(texture *t) {
  int n_cells = t->rows * t->columns;
  bzero(t->array, n_cells * sizeof(*t->array));
}

void texture_fill_rand(texture *t, xorshift64s *r) {
  int cell_left = t->rows * t->columns;
  f32 *array = t->array;
  while(cell_left--) {
    *array++ = prng_next_f64(r);
  }
}

void texture_line_add(f32* texture, f32 *line, int n) {
  for (int i = 0; i < n; i++) {
    *(texture + i) += *(line + i);
  }
}

void texture_line_fill_step1_rec(f32 *line, int n, int z, xorshift64s *r) {
  if (z == 0) {
    *line = prng_next_f64(r);
    return;
  }
  texture_line_fill_step1_rec(line, n/2, z - 1, r);
  texture_line_fill_step1_rec(line + n/2, n/2, z - 1, r);
}

int cycle(int *it, int max) {
  int i = *it;
  i++;
  if (i == max) {
    i = 0;
  }
  *it = i;
  return i;
}

void texture_line_fill(f32 *line, int n, int z, xorshift64s *r) {
  texture_line_fill_step1_rec(line, n, z, r);
  f32 *end = line + n;
  int i = 0;
  f32 f;
  while (line < end) {
    if (!cycle(&i, z)) {
      f = *line;
    }
    *line++ = f;
  }
}

void texture_fill_perlin(texture *t, int zoom, xorshift64s *r) {
  int n_cells = t->rows * t->columns;
  int block_size = t->rows / zoom;
  f32 line[256];
  assert(sizeof(line) / sizeof(*line) >= t->columns);
  for (int outer = 0; outer < block_size; outer++) {
    int block_offset = outer * zoom * t->columns;
    for (int z = 0; z < zoom; z++) {
      texture_line_fill(line, t->columns, z, r);
      for (int inner = 0; inner < z; inner++) {
        int offset = inner * t->columns + block_offset;
        if (offset > n_cells) {
          break; // next z
        }
        texture_line_add(t->array + offset, line, t->columns);
      }
    }
  }
}

void texture_print(texture *t) {
  int i = 0;
  int end = t->rows * t->columns;
  size_t s = 16 * t->columns + 1;
  size_t n = s;
  char buffer[s];
  char *b = buffer;
  while (i < end) {
    int wrote = snprintf(b, n, "%e, ", *(t->array + i));
    b += wrote;
    s -= wrote;
    i++;
    if (i % t->columns == 0) {
      *(b - 2) = 0; // erase last ", "
      b = buffer;
      n = s;
      puts(b);
    }
  }
}

texture *the_texture;

void draw_the_texture() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3fv((f32*)&blue);

  //mesh_draw(&the_tree, disp_wind);

  glFlush();
  gl_redisplay();
}

int main(int argc, char **args) {
  struct glconfig gl_setup_cfg = (struct glconfig) {
    .n      = argc,
    .args   = args,
    .w      = 512,
    .h      = 512,
    .x      = 0,
    .y      = 0,
    .title  = "hello world",
  };
  const struct glinit gl_screen_cfg = (struct glinit) {
    .screen_left   = -5,
    .screen_right  = 55.0,
    .screen_bottom = -5,
    .screen_top    = 55.0,
    .screen_near   = -1,
    .screen_far    = +1,
  };
//  gl_setup(&gl_setup_cfg);
//  gl_init(&gl_screen_cfg);

  int rows = 4;
  int columns = 4;
  xorshift64s r = 42;
  texture *t = texture_make(rows, columns);
  texture_clear(t);
  //texture_fill_perlin(t, 4, &r);
  texture_print(t);
  the_texture = t;

//  gl_display(draw_the_texture);
//  gl_loop();

  free(t);
}
