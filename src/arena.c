#include "../include/collections/arena.h"

#define FLAG_ENABLED(arena, flag) ((arena->flags & flag) == flag)

typedef struct arena_t arena_t;
typedef struct mem_node mem_node;

struct mem_node {
  void *memory; // 8
  mem_node *next; // 8
  size_t size; // 8
  size_t used; // 8
};

struct arena_t {
  void *pos; // 8
  mem_node *head; // 8
  mem_node *cur; // 8
  size_t size; // 8
  size_t used; // 8
  int flags; // 4
};

static mem_node *create_memory_node(size_t size) {
  mem_node *new_block = (mem_node *)malloc(sizeof(mem_node));
  if (new_block == NULL) {
    return NULL;
  }
  new_block->memory = malloc(size);
  if (new_block->memory == NULL) {
    free(new_block);
    return NULL;
  }
  new_block->size = size;
  new_block->used = 0;
  new_block->next = NULL;

  return new_block;
}

arena_t *arena_create(size_t size) {
  return arena_create_flags(size, ARENA_DEFAULT_FLAGS);
}

arena_t *arena_create_flags(size_t size, int flags) {
  arena_t *new_arena = (arena_t *)malloc(sizeof(arena_t));
  if (new_arena == NULL) {
    if ((flags & ARENA_PRINT_DEBUG) == ARENA_PRINT_DEBUG) printf("ARENA : Failed to allocate arena struct!\n");
    if ((flags & ARENA_EXIT_ON_ERROR) == ARENA_EXIT_ON_ERROR) exit(1);
    return NULL;
  }
  new_arena->head = create_memory_node(size);
  if (new_arena->head == NULL) {
    if ((flags & ARENA_PRINT_DEBUG) == ARENA_PRINT_DEBUG) printf("ARENA %lx : Failed to create initial arena memory node!\n", (uintptr_t)new_arena);
    free(new_arena);
    if ((flags & ARENA_EXIT_ON_ERROR) == ARENA_EXIT_ON_ERROR) exit(1);
    return NULL;
  }
  new_arena->flags = flags;
  new_arena->cur = new_arena->head;
  new_arena->pos = new_arena->cur->memory;
  new_arena->size = size;
  new_arena->used = 0;

  return new_arena;
}

void arena_free(arena_t *arena) {
  if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Freeing!\n", (uintptr_t)arena);
  // First free memory nodes
  for (mem_node *current = arena->head; current != NULL;) {
    free(current->memory);
    mem_node *prev = current;
    current = current->next;
    free(prev);
  }
  free(arena);
  return;
}

static void *align_ptr(void *ptr, size_t align) {
  uintptr_t addr = (uintptr_t)ptr;
  uintptr_t aligned = (addr + align - 1) & ~(align - 1);
  return (void*)aligned;
}

static bool is_power_of_2(size_t n) {
  return n > 0 && (n  & (n - 1)) == 0;
}

void *arena_alloc_aligned(arena_t *arena, size_t size, size_t align) {
  // Ensure it's a power of 2
  // Not doing ARENA_EXIT_ON_ERROR as it's a simple mis-use error
  if (!is_power_of_2(align)) { return NULL; }

  if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Trying to assign %lu with %lu alignment\n", (uintptr_t)arena, size, align);

  mem_node *current = arena->cur;

  // Figure out how much we need including alignment
  void *aligned_arena_pos = align_ptr(arena->pos, align);
  size_t padding_required = (uintptr_t)aligned_arena_pos - (uintptr_t)arena->pos;
  size_t total_size_required = size + padding_required;
   
  // Check if we have space in the current mem_node
  if (current->size < total_size_required + current->used) {
    if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Not enough space!\n", (uintptr_t)arena);
    if (!FLAG_ENABLED(arena, ARENA_GROWABLE)) {
      if (FLAG_ENABLED(arena, ARENA_EXIT_ON_ERROR)) exit(1);
      return NULL;
    }
    // AS the arena can be reset, check if we have a next node already.
    if (current->next != NULL) {
      if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Next node already exists!\n", (uintptr_t)arena);
      // Signify we've used the entire block
      arena->used += current->size - current->used;
      current->used = current->size;

      // Move to the next block
      arena->cur = current->next;
      arena->pos = arena->cur->memory;
      if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Moved to next memory node!\n", (uintptr_t)arena);

      // Try again
      return arena_alloc_aligned(arena, size, align);
    }
    
    if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Creating a new memory node!\n", (uintptr_t)arena);
    // Create a new mem_node, twice the size
    size_t extension = current->size*2;
    mem_node *new_node = create_memory_node(extension);
    if (new_node == NULL) {
      if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Failed to create memory node", (uintptr_t)arena);
      if (FLAG_ENABLED(arena, ARENA_EXIT_ON_ERROR)) exit(1);
      return NULL;
    }

    if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Updating old memory node to suggest it's full\n", (uintptr_t)arena);

    // Signify we've used the entire block
    arena->used += current->size - current->used;
    current->used = current->size;
    // Extend the arena size
    arena->size += extension;

    // Move to the next block
    arena->cur->next = new_node;
    arena->cur = new_node;
    arena->pos = arena->cur->memory;
    if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Moved to next memory node!\n", (uintptr_t)arena);

    // Try again
    return arena_alloc_aligned(arena, size, align);
  }

  // Save the pointer before moving it
  arena->pos += total_size_required;
  arena->used += total_size_required;
  arena->cur->used += total_size_required;

  return aligned_arena_pos;
}

void *arena_alloc(arena_t *arena, size_t size) {
  return arena_alloc_aligned(arena, size, alignof(max_align_t));
}

void arena_reset(arena_t *arena) {
  // Reset the backing memory nodes
  for (mem_node *cur = arena->head; cur != NULL; cur = cur->next) {
    memset(cur->memory, 0, cur->size);
    cur->used = 0;
  }

  // Set pointer back to the very start
  arena->pos = arena->head->memory;
  arena->cur = arena->head;

  // Reset arena metadata
  arena->used = 0;

  if (FLAG_ENABLED(arena, ARENA_PRINT_DEBUG)) printf("ARENA %lx : Reset!\n", (uintptr_t)arena);
}
