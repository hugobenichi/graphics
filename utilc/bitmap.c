//#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* easy_file_open(const char * filename) {
  FILE* out = fopen(filename, "wb");
  if (!out) {
      char msg[128] = "could not open file ";
      strlcat(msg, filename, sizeof(msg));
      perror(msg);
  }
  return out;
}

typedef uint8_t u8;
typedef int8_t i8;

struct RGB {
  u8 r;
  u8 g;
  u8 b;
};

typedef struct RGB rgb24;

struct RGBA {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

typedef struct RGBA rgba32;

struct bitmap {
  int width;
  int height;
  int stride;   // byte spacing between pixels
  u8* pixels;    // from top left corner to bottom right corner, width innermost.
};

int bitmap_new(struct bitmap* c, int width, int height, int pixel_size) {
  u8* ptr = malloc(pixel_size * width * height);
  c -> width  = width;
  c -> height = height;
  c -> stride = pixel_size;
  c -> pixels = ptr;
  return ptr == NULL;
}

#define _addr_shift(ptr, offset)  (void*)( ((u8*) ptr) + offset )

int saturate_u8(int x) {
  if (x > 255) return 255;
  if (x < 0) return 0;
  return x;
}

static const char padding[4] = {0};

void bitmap_write(struct bitmap* bm, FILE* out) {
  // bitmap format tag
  fwrite("BM", 2, 1, out);

  // bitmap header
  const int header_size = 54;
  const int width_size = 3 * bm->width;
  const int is_line_size_multiple_of_4 = (width_size & 3) == 0;
  const int line_padding = is_line_size_multiple_of_4 ? 0 : 4 - (width_size & 3);
  const int data_size = (width_size + line_padding) * bm->height;
  const int total_size = header_size + data_size;
  const unsigned int headers[13] = {
    total_size,                     // bfSize (whole file size)
    0,                              // bfReserved (both)
    header_size,                    // bfOffbits
    40,                             // biSize
    bm->width,                      // biWidth
    bm->height,                     // biHeight
    1 + (24 << 16),                 // biPlanes (short), biBitCount (short)
    0,                              // biCompression
    data_size,                      // biSizeImage
    0,                              // biXPelsPerMeter
    0,                              // biYPelsPerMeter
    0,                              // biClrUsed
    0,                              // biClrImportant
  };
  fwrite(headers, sizeof(headers), 1, out);

  // bitmap data
  u8* pixel = bm->pixels;
  for (int y = bm->height - 1; y >= 0; y--) {    // bitmap data is written from bottom to top
    for (int x = 0; x < bm->width; x++) {
      rgb24 p = *(rgb24*)(pixel + (y * bm->width + x) * bm->stride);
      u8 inversed_pixel[3] = {p.b, p.g, p.r};     // bitmap pixel order is blue,green,red
      fwrite(&inversed_pixel, sizeof(inversed_pixel), 1, out);
    }
    fwrite(headers, line_padding, 1, out); // complete line so that its size is a multiple of 4
  }
}

void fill_texture_1(struct bitmap* bm) {
  rgb24* out = (rgb24*) bm->pixels;
  const float omega = 0.1;
  for (int x = 0; x < bm->height; x++)
  for (int y = 0; y < bm->width; y++) {
    u8 m = saturate_u8(128 + 128 * sin(omega * x * y));
    *out = (rgb24) {
      .b = m,
      .g = m,
      .r = m,
    };
    out = _addr_shift(out, bm->stride);
  }
}

void fill_texture_2(struct bitmap* bm) {
  rgb24* out = (rgb24*) bm->pixels;
  const float omega = 0.1;
  for (int x = 0; x < bm->height; x++)
  for (int y = 0; y < bm->width; y++) {
    u8 m = saturate_u8(128 + 128 * sin(omega * (x + y)));
    *out = (rgb24) {
      .b = m,
      .g = m,
      .r = m,
    };
    out = _addr_shift(out, bm->stride);
  }
}

int make_texture() {
  struct bitmap bm;
  int err = bitmap_new(&bm, 256, 256, sizeof(rgb24));
  if (err) {
    return err;
  }
  FILE* f = easy_file_open("texture.bmp");
  if (!f) return -1;
  fill_texture_2(&bm);
	bitmap_write(&bm, f);
  fclose(f);
  return 0;
}

int main(int argc, char * argv[]) {
  make_texture();
}
