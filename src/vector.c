#include "../include/collections/vector.h"

// Over-allocation strategy from CPython list implementation
// A mild over-allocation strategy, but is enough to give linear-time
// amortised behaviour over a long sequence of appends()
#define VECTOR_GROW(cap) (cap + cap / 8) + (cap < 9 ? 3 : 9)

typedef c_vector_t vector_t;

struct vector_t {
  void *mem; // 8
  size_t size; // 8
  size_t capacity; // 8
  size_t elem_size; // 8
};

vector_t *vector_create(size_t elem_size) {
  if (elem_size == 0) return NULL;

  size_t beginning_cap = 3;

  vector_t *vec = (vector_t*)malloc(sizeof(vector_t));
  if (vec == NULL) return NULL;
  // Start with size for 3 elements (over-allocation for ammortised cost)
  vec->mem = malloc(elem_size * beginning_cap);
  if (vec->mem == NULL) {
    free(vec);
    return NULL;
  }

  vec->capacity = beginning_cap;
  vec->size = 0;
  vec->elem_size = elem_size;

  return vec;
}

void vector_free(vector_t *vector) {
  if (vector == NULL) return;
  free(vector->mem);
  free(vector);
}

int vector_set(vector_t *vector, size_t index, const void *item) {
  if (vector == NULL) return -1;
  if (item == NULL) return -1;

  // size_t cannot be negative
  if (index >= vector->size) return -1;

  char *memptr = (char*)vector->mem;
  // Copy it in
  memcpy(&memptr[index * vector->elem_size], item, vector->elem_size);

  return 0;
}

int vector_get(const vector_t *vector, size_t index, void *out) {
  if (vector == NULL) return -1;
  if (out == NULL) return -1;

  if (index >= vector->size) return -1;

  char *memptr = (char*)vector->mem;
  memcpy(out, &memptr[index * vector->elem_size], vector->elem_size);

  return 0;
}

int vector_insert(vector_t *vector, size_t index, const void *item) {
  if (vector == NULL) return -1;
  if (item == NULL) return -1;

  // size_t cannot be negative
  if (vector->size < index) return -1;

  if (vector->capacity == vector->size) {
    // Need to resize
    size_t grow_to = VECTOR_GROW(vector->capacity);
    void *new_mem = realloc(vector->mem, grow_to * vector->elem_size);
    if (new_mem == NULL) return -1;
    vector->mem = new_mem;
    vector->capacity = grow_to;
  }

  char *memptr = (char*)vector->mem;

  // Move memory forward to make room
  memmove(&memptr[vector->elem_size * (index + 1)], &memptr[vector->elem_size * index], vector->elem_size * (vector->size - index));

  // Copy the value of the item in
  memcpy(&memptr[index * vector->elem_size], item, vector->elem_size);
  vector->size++;

  return 0;
}

int vector_remove(vector_t *vector, size_t index, void *out) {
  if (vector == NULL) return -1;
  if (vector->size == 0) return -1;
  if (index >= vector->size) return -1;

  char *memptr = (char*)vector->mem;
  if (out != NULL) {
    memcpy(out, &memptr[index * vector->elem_size], vector->elem_size);
  }

  // Move things back to fill the empty space
  memmove(&memptr[index * vector->elem_size], &memptr[(index + 1) * vector->elem_size], (vector->size - index - 1) * vector->elem_size);

  vector->size--;

  return 0;
}

int vector_push_back(vector_t *vector, const void *item) {
  return vector_insert(vector, vector->size, item);
}

int vector_pop_back(vector_t *vector, void *out) {
  return vector_remove(vector, vector->size - 1, out);
}

int vector_reserve(vector_t *vector, size_t capacity) {
  if (vector == NULL) return -1;
  if (capacity < vector->capacity) return -1;
  if (capacity == vector->capacity) return 0;

  void *new_mem = realloc(vector->mem, capacity * vector->elem_size);
  if (new_mem == NULL) return -1;
  vector->mem = new_mem;
  vector->capacity = capacity;

  return 0;
}

int vector_resize(vector_t *vector, size_t size, const void *default_value) {
  if (vector == NULL) return -1;
  if (size == vector->size) return 0;

  // Ensure we have enough capacity
  if (size > vector->capacity) {
    // Reserve enough space + over-allocation
    if (vector_reserve(vector, VECTOR_GROW(size)) == -1) return -1;
  }

  // If we're expanding
  if (size > vector->size) {
    // Initialise everything to the default value
    char *memptr = (char*)vector->mem;
    for (size_t i = vector->size; i < size; i++) {
      memcpy(&memptr[i * vector->elem_size], default_value, vector->elem_size);
    }
  }
  // Don't need to do anything for shrinking, simply changing the size
  // value makes further ones inaccessible via `get`
  vector->size = size;

  return 0;
}

size_t vector_size(const vector_t *vector) {
  if (vector == NULL) return 0;
  return vector->size;
}

size_t vector_capacity(const vector_t *vector) {
  if (vector == NULL) return 0;
  return vector->capacity;
}

bool vector_empty(const vector_t *vector) {
  if (vector == NULL) return false;
  return vector->size == 0;
}
