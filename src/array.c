#include "../include/collections/array.h"
#include "../include/collections/arena.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/*
 * Not using a dynamic arena to back array, as it needs to be
 * contiguous in virtual memory.
 *
 * Standard arena also isn't required for a simple array implementation,
 * easier to just manually handle memory due to memory operations.
 */

// Over-allocation strategy from CPython list implementation
// A mild over-allocation strategy, but is enough to give linear-time
// amortised behaviour over a long sequence of appends()
#define CALCULATE_RESIZE(size) size + (size / 8) + (size < 9 ? 3 : 6)

typedef c_array_t array_t;

struct array_t {
  void **items;
  size_t length;
  size_t allocation_size;
};

array_t *array_create() {
  array_t *new_arr = (array_t *)malloc(sizeof(array_t));
  if (new_arr == NULL) {
    return NULL;
  }

  size_t over_allocation = CALCULATE_RESIZE(0);

  // Just storing pointers
  new_arr->items = malloc(sizeof(void *) * over_allocation);
  if (new_arr->items == NULL) {
    free(new_arr);
    return NULL;
  }
  memset(new_arr->items, 0, sizeof(void*) * over_allocation);

  new_arr->length = 0;
  new_arr->allocation_size = over_allocation;

  return new_arr;
}

void array_free(array_t *array) {
  free(array->items);
  free(array);
}

// Return the item on succesful append, or NULL on error
void *array_append(array_t *array, void *item) {

  // Check if we need to resize
  if (array->length == array->allocation_size) {
    printf("Re-allocating space...\n");
    size_t new_capacity = CALCULATE_RESIZE(array->allocation_size);
    void **new_items = realloc(array->items, sizeof(void*) * new_capacity);
    if (new_items == NULL) {
      return NULL;
    }
    printf("Went from %ld -> %ld\n", array->length, new_capacity);
    array->items = new_items;

    // Initialise new slots to null
    memset(&array->items[array->allocation_size], 0, sizeof(void*) * (new_capacity - array->allocation_size));

    array->allocation_size = new_capacity;
    printf("Re-allocated!\n");
  }

  printf("Storing pointer...\n");
  array->items[array->length] = item;
  printf("Stored pointer!\n");
  array->length++;

  return array->items[array->length - 1];
}

void *array_get(array_t *array, size_t index) {
  if (index + 1 > array->length || index < 0) {
    return NULL;
  }
  return array->items[index];
}

int main(void) {
  c_arena_t *tmp = arena_create_flags(1024, ARENA_EXIT_ON_ERROR | ARENA_GROWABLE);
  array_t *myarr = array_create();

  for (int i = 0; i < 100; i++) {
    int *num = arena_alloc(tmp, sizeof(int));
    if (num == NULL) {
      printf("Failed to allocate memory from ARENA!\n");
      exit(1);
    }
    *num = i;
    printf("appending '%d'...\n", i);
    int *ret = array_append(myarr, num);
    if (ret == NULL) {
      printf("FAILED TO APPEND '%d'!\n", i);
    }
    printf("Yippee!!!\n");
  }

  for (int i = 0; i < 100; i++) {
    printf("arr[%d] = %d\n", i, *(int*)array_get(myarr, i));
  }

  printf("Freeing array\n");
  array_free(myarr);  
  arena_free(tmp);
  return 0;
}
