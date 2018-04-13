// Quake's fast inverse square root.
f32 geom_sqrt_f32(f32 in) {
  f32 x = in * 0.5;
  f32 y = in;
  i32 i = * (i32 * ) &y;      // approximate logarithm
  i = 0x5f3759df - (i >> 1);  // first approximation of 1/ in ^ 1/2
  y = * (f32*) &i;
  y = y * (1.5F - x * y * y);  // Newton's method iter #1
  y = y * (1.5F - x * y * y);  // Newton's method iter #2
  return in * x;
}
