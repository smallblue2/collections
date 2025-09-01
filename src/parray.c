#include "../include/collections/parray.h"

/*
 * Not using a growable arena to back pointer array, as it needs to be
 * contiguous in virtual memory for arbitrary insertions.
 */

// Over-allocation strategy from CPython list implementation
// A mild over-allocation strategy, but is enough to give linear-time
// amortised behaviour over a long sequence of appends()
#define CALCULATE_RESIZE(size) size + (size / 8) + (size < 9 ? 3 : 6)

typedef c_parray_t parray_t;

struct parray_t {
  void **items;
  size_t length;
  size_t allocation_size;
};

parray_t *array_create() {
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

  return new_arr;
}

void array_free(parray_t *array) {
  for (size_t i = 0; i < array->length; i++) {
    free(array->items[i]);
  }
  free(array->items);
  free(array);
}

bool __array_grow(parray_t *array) {
  size_t new_capacity = CALCULATE_RESIZE(array->allocation_size);
  void **new_items = realloc(array->items, sizeof(void*) * new_capacity);
  if (new_items == NULL) {
    return false;
  }
  array->items = new_items;
  memset(&array->items[array->allocation_size], 0, sizeof(void*) * (new_capacity - array->allocation_size));
  array->allocation_size = new_capacity;

  return true;
}

// Return the item on succesful append, or NULL on error
const void *array_append(parray_t *array, const void *item, const size_t size) {
  // Allocate memory first incase it fails
  void *new_item = malloc(size);
  if (new_item == NULL) {
    return NULL;
  }

  // Check if we need to resize
  if (array->length == array->allocation_size) {
    if (!__array_grow(array)) {
      free(new_item);
      return NULL;
    }
  }

  // Copy object by value
  array->items[array->length] = new_item;
  memcpy(array->items[array->length], item, size);

  array->length++;

  return array->items[array->length - 1];
}

const void *array_get(parray_t *array, size_t index) {
  if (index + 1 > array->length || index < 0) {
    return NULL;
  }
  return array->items[index];
}

size_t array_length(parray_t *array) { return array->length; };

// Only allows insertion from 0 < length
const void *array_insert(parray_t *array, size_t index, const void *item, size_t size) {
  // Check if the insertion is within bounds
  if (index < 0 || array->length < index) {
    return NULL;
  }

  // If it's inserted to the end, it's just an append
  if (index == array->length) return array_append(array, item, size);

  // Allocate memory for new item early incase it fails
  void *new_item = malloc(size);
  if (new_item == NULL) {
    return NULL;
  }

  // Resize if we need to
  if (array->length + 1 == array->allocation_size) {
    if (!__array_grow(array)) {
      free(new_item);
      return NULL;
    }
  }

  // Move memory forward
  memmove(&array->items[index + 1], &array->items[index], sizeof(void*) * (array->length - index));

  // Copy value and place it in our array
  array->items[index] = new_item;
  memcpy(array->items[index], item, size);

  array->length += 1;

  return array->items[index];
}

// User has ownership over this now
void *array_pop(parray_t *array, size_t index) {
  // Ensure we're within bounds
  if (index < 0 || array->length < index) {
    return NULL;
  }

  // Keep a reference to the item incase we overwrite the index
  void *to_remove = array->items[index];
  array->length--;

  // If it's the final item, we don't have to move any memory
  if (index == array->length) {
    return to_remove;
  }

  // Move the memory back
  memmove(&array->items[index], &array->items[index + 1], sizeof(void*) * (array->length - index));

  return to_remove;
}

int main(void) {
  parray_t *myarr = array_create();
  
  for (int i = 0; i < 100; i++) {
    printf("appending '%d'...\n", i);
    const int *ret = (int*)array_append(myarr, &i, sizeof i);
    if (ret == NULL) {
      printf("FAILED TO APPEND '%d'!\n", i);
    }
    printf("Yippee!!!\n");
  }

  array_insert(myarr, 0, "MEOW", strlen("MEOW") + 1);

  char *msg = (char*)array_pop(myarr, 0);
  printf("msg: %s\n", msg);
  free(msg);

  for (int i = 0; i < myarr->length; i++) {
    if (i == -1) printf("arr[%d] = %s\n", i, (char*)array_get(myarr, i));
    else printf("arr[%d] = %d\n", i, *(int*)array_get(myarr, i));
  }

  printf("Freeing array\n");
  array_free(myarr);  
  return 0;
}
