#include "../include/allocators/arena.h"
#include <stddef.h>
#include <stdint.h>

typedef alctr_arena_t arena_t;

struct arena_t {
  void *memory;
  void *pos;
  size_t size;
  size_t used;
};


arena_t *arena_create(size_t size) {
  arena_t *arena = (arena_t*)malloc(sizeof(arena_t));
  if (arena == nullptr) { return nullptr; }

  arena->memory = (void*)malloc(size);
  if (arena->memory == nullptr) {
    free(arena);
    return nullptr;
  }

  // Zero our memory block
  memset(arena->memory, 0, size);
  arena->pos = arena->memory;
  arena->size = size;
  arena->used = 0;

  return arena;
}

void *arena_alloc_aligned(arena_t *arena, size_t size, size_t align) {
  // Convert pointer to integer for bitwise arithmetic
  uintptr_t current = (uintptr_t)arena->pos;
  // Bitmask magic to magically round up to the closest multiple of `align`
  uintptr_t aligned = (current + (align - 1)) & ~(align - 1);
  arena->pos = (void*)aligned;
  arena->used = aligned - (uintptr_t)arena->memory;
  return (void*)current;
}

void *arena_alloc(arena_t *arena, size_t size) {
  return arena_alloc_aligned(arena, size, alignof(max_align_t));
}

void arena_free(arena_t *arena) {
  free(arena->memory);
  free(arena);
  return;
}

void arena_reset(arena_t *arena) {
  arena->pos = arena->memory;
  arena->used = 0;
  memset(arena->memory, 0, arena->size);
}
