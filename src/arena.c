#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct arena_t arena_t;
typedef struct mem_node mem_node;

struct mem_node {
  void *memory;
  mem_node *next;
  size_t size;
  size_t used;
};

struct arena_t {
  void *pos;
  mem_node *head;
  mem_node *cur;
  size_t size;
  size_t used;
};

mem_node *create_memory_node(size_t size) {
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
  arena_t *new_arena = (arena_t *)malloc(sizeof(arena_t));
  if (new_arena == NULL) {
    return NULL;
  }
  new_arena->head = create_memory_node(size);
  if (new_arena->head == NULL) {
    free(new_arena);
    return NULL;
  }
  new_arena->cur = new_arena->head;
  new_arena->pos = new_arena->cur->memory;
  new_arena->size = size;
  new_arena->used = 0;

  return new_arena;
}

void arena_free(arena_t *arena) {
  // First free memory nodes
  for (mem_node *current = arena->head; current != NULL;) {
    free(current->memory);
    mem_node *prev = current;
    current = current->next;
    free(prev);
  }
  free(arena);
  arena = NULL;
  return;
}

void *arena_alloc(arena_t *arena, size_t size) {
  printf("Allocating!\n");
  mem_node *current = arena->cur;
  // Check if we have space in the current mem_node
  if (current->size < current->used + size) {
    printf("Not enough space!\n");
    // AS the arena can be reset, check if we have a next node already.
    if (current->next != NULL) {
      printf("Next node already exists!\n");
      // Signify we've used the entire block
      arena->used += current->size - current->used;
      current->used = current->size;

      // Move to the next block
      arena->cur = current->next;
      arena->pos = arena->cur->memory;
      printf("Moved to next node!\n");

      // Try again
      return arena_alloc(arena, size);
    }
    
    printf("Creating a new node!\n");
    // Create a new mem_node, twice the size
    size_t extension = current->size*2;
    mem_node *new_node = create_memory_node(extension);
    if (new_node == NULL) {
      return NULL;
    }

    printf("Updating old node to suggest it's full\n");

    // Signify we've used the entire block
    arena->used += current->size - current->used;
    current->used = current->size;
    // Extend the arena size
    arena->size += extension;

    // Move to the next block
    arena->cur->next = new_node;
    arena->cur = new_node;
    arena->pos = arena->cur->memory;
    printf("Moved to next node!\n");

    // Try again
    return arena_alloc(arena, size);
  }

  // Save the pointer before moving it
  void *new_ptr = arena->pos;
  arena->pos += size;
  arena->used += size;
  arena->cur->used += size;

  return new_ptr;
}

void arena_reset(arena_t *arena) {
  // Reset the backing memory nodes
  for (mem_node *cur = arena->head; cur != NULL;) {
    memset(cur->memory, 0, cur->size);
    cur->used = 0;
    cur = cur->next;
  }

  // Set pointer back to the very start
  arena->pos = arena->head->memory;
  arena->cur = arena->head;

  // Reset arena metadata
  arena->used = 0;

  printf("RESET!\n");
}

void arena_print_stats(arena_t *arena) {
  printf("================== STATS ==================\n");
  printf("ARENA:\n");
  printf("\tused: %lu\n\tsize: %lu\n\tnodes:\n", arena->used, arena->size);
  int node_count = 0;
  for(mem_node *cur = arena->head; cur != NULL;) {
    printf("\t\t%d:\n", node_count);
    printf("\t\t\tused: %lu\n\t\t\tsize: %lu\n", cur->used, cur->size);
    node_count++;
    cur = cur->next;
  }
  printf("===========================================\n");
}

int main(void) {
  arena_t *arena = arena_create(4);
  printf("Created!\n");

  char *zoe = arena_alloc(arena, 4);
  strncpy(zoe, "zoe", 4);
  arena_print_stats(arena);

  char *niall = arena_alloc(arena, 6);
  strncpy(niall, "niall", 6);
  arena_print_stats(arena);

  char *hw = arena_alloc(arena, 12);
  strncpy(hw, "hello world", 12);
  arena_print_stats(arena);

  arena_reset(arena);

  char *zoe1 = arena_alloc(arena, 4);
  strncpy(zoe1, "zoe", 4);
  arena_print_stats(arena);

  char *niall1 = arena_alloc(arena, 6);
  strncpy(niall1, "niall", 6);
  arena_print_stats(arena);

  char *hw1 = arena_alloc(arena, 12);
  strncpy(hw1, "hello world", 12);
  arena_print_stats(arena);

  printf("0: %s\n1: %s\n2: %s\n", zoe, niall, hw);

  arena_free(arena);
  printf("Freed!\n");

  return 0;
}
