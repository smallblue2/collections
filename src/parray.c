#include "../include/collections/parray.h"

/*
 * Not using a growable arena to back pointer array, as it needs to be
 * contiguous in virtual memory for arbitrary insertions.
 */

// Over-allocation strategy from CPython list implementation
// A mild over-allocation strategy, but is enough to give linear-time
// amortised behaviour over a long sequence of appends()
#define CALCULATE_RESIZE(size) (size + (size / 8) + (size < 9 ? 3 : 6))

typedef c_parray_t parray_t;

struct parray_t {
  void **items; // 8
  size_t length; // 8
  size_t allocation_size; // 8
  void (*parray_free_func)(void*);
};

parray_t *parray_create(void (*parray_free_func)(void*)) {
  parray_t *new_arr = (parray_t *)malloc(sizeof(parray_t));
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
  new_arr->parray_free_func = parray_free_func;

  return new_arr;
}

void parray_free(parray_t *parray) {
  if (parray->parray_free_func != NULL) {
    for (size_t i = 0; i < parray->length; i++) {
      parray->parray_free_func(parray->items[i]);
    }
  }
  free(parray->items);
  free(parray);
}

int __parray_grow(parray_t *parray) {
  size_t new_capacity = CALCULATE_RESIZE(parray->allocation_size);
  void **new_items = realloc(parray->items, sizeof(void*) * new_capacity);
  if (new_items == NULL) return -1;
  parray->items = new_items;
  memset(&parray->items[parray->allocation_size], 0, sizeof(void*) * (new_capacity - parray->allocation_size));
  parray->allocation_size = new_capacity;

  return 0;
}

// Return the item on succesful append, or NULL on error
int parray_append(parray_t *parray, void *item) {
  // Check if we need to resize
  if (parray->length == parray->allocation_size) {
    if (__parray_grow(parray) != 0) {
      return -1;
    }
  }

  // Store pointer in place
  parray->items[parray->length] = item;

  parray->length++;

  return 0;
}

const void *parray_get(const parray_t *parray, size_t index) {
  if (index >= parray->length) {
    return NULL;
  }
  return parray->items[index];
}

size_t parray_length(const parray_t *parray) { return parray->length; };

// Only allows insertion from 0 < length
int parray_insert(parray_t *parray, size_t index, void *item) {
  // Check if the insertion is within bounds
  if (parray->length < index) {
    return -1;
  }

  // If it's inserted to the end, it's just an append
  if (index == parray->length) return parray_append(parray, item);

  // Resize if we need to
  if (parray->length == parray->allocation_size) {
    if (__parray_grow(parray) != 0) {
      return -1;
    }
  }

  // Move memory forward
  memmove(&parray->items[index + 1], &parray->items[index], sizeof(void*) * (parray->length - index));

  // Store in place
  parray->items[index] = item;

  parray->length += 1;

  return 0;
}

// User has ownership over this now
void *parray_pop(parray_t *parray, size_t index) {
  // Ensure we're within bounds
  if (index < 0 || parray->length <= index) {
    return 0;
  }

  // Keep a reference to the item incase we overwrite the index
  void *to_remove = parray->items[index];
  parray->length--;

  // Move the memory back
  memmove(&parray->items[index], &parray->items[index + 1], sizeof(void*) * (parray->length - index));

  return to_remove;
}
