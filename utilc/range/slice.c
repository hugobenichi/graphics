struct slice {
  void* mem;
  int len;
  int max;
}

void slice_alloc(struct slice* s, int size) {
  assert(size >= 0);
  s -> len = size;
  s -> max = size;
  s -> mem = malloc(size);
}

void slice_append(struct slice* s, void* elem, int elem_size) {
  if (s -> o)
}
