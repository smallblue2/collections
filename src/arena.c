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
  if (arena == NULL) { return NULL; }

  arena->memory = (void*)malloc(size);
  if (arena->memory == NULL) {
    free(arena);
    return NULL;
  }

  // Zero our memory block
  memset(arena->memory, 0, size);
  arena->pos = arena->memory;
  arena->size = size;
  arena->used = 0;

  return arena;
}

void *arena_alloc_aligned(arena_t *arena, size_t size, size_t align) {
  uintptr_t current = (uintptr_t)arena->pos;
  // Ensure the start is aligned
  uintptr_t aligned = (current + (align - 1)) & ~(align - 1);

  // Ensure we're within bounds
  if (aligned + size > (uintptr_t)arena->memory + arena->size) return NULL;
  arena->pos = (void*)(aligned + size);
  arena->used = (uintptr_t)arena->pos - (uintptr_t)arena->memory;
  return (void*)aligned;
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
