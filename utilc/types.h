#pragma once

#include <assert.h>
#include <stdint.h>

typedef float     f32;
typedef double    f64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

#define _define_tuple2_of(name, type)  struct name {\
                                        type x; \
                                        type y; \
                                      }
#define _define_tuple3_of(name, type)  struct name {\
                                        type x; \
                                        type y; \
                                        type z; \
                                      }
#define _define_tuple4_of(name, type)  struct name {\
                                        type x; \
                                        type y; \
                                        type z; \
                                        type t; \
                                      }

_define_tuple2_of(Tuple2_Of_F32, f32);
_define_tuple3_of(Tuple3_Of_F32, f32);
_define_tuple4_of(Tuple4_Of_F32, f32);

_define_tuple2_of(Tuple2_Of_F64, f64);
_define_tuple3_of(Tuple3_Of_F64, f64);
_define_tuple4_of(Tuple4_Of_F64, f64);

#undef _define_tuple2_of
#undef _define_tuple3_of
#undef _define_tuple4_of

// Complex numbers
typedef struct Tuple2_Of_F32 c64;
typedef struct Tuple2_Of_F64 c128;

// 2D & 3D Vectors
typedef struct Tuple2_Of_F32 v2_64;
typedef struct Tuple3_Of_F32 v3_96;
typedef struct Tuple4_Of_F32 v4_128;

typedef struct Tuple2_Of_F64 v2_128;
typedef struct Tuple3_Of_F64 v3_192;
typedef struct Tuple4_Of_F64 v4_256;

// TODO: Quaternions ...

// TODO: RGB pixels ...

// TODO: RGBA pixels ...

static void geom_tests() {
  #define _assert_sizeof(t, s) assert(sizeof(t) == s)
  _assert_sizeof(i8, 1);
  _assert_sizeof(i16, 2);
  _assert_sizeof(i32, 4);
  _assert_sizeof(i64, 8);

  _assert_sizeof(u8, 1);
  _assert_sizeof(u16, 2);
  _assert_sizeof(u32, 4);
  _assert_sizeof(u64, 8);

  _assert_sizeof(f32, 4);
  _assert_sizeof(f64, 8);

  _assert_sizeof(c64, 8);
  _assert_sizeof(c128, 16);

  _assert_sizeof(v2_64, 8);
  _assert_sizeof(v3_96, 12);
  _assert_sizeof(v4_128, 16);

  _assert_sizeof(v2_128, 16);
  _assert_sizeof(v3_192, 24);
  _assert_sizeof(v4_256, 32);
  #undef _assert_sizeof
}
