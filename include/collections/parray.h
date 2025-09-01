#ifndef PARRAYH
#define PARRAYH

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct parray_t c_parray_t;

c_parray_t *array_create();
void array_free(c_parray_t *array);
const void *array_append(c_parray_t *array, const void *item, const size_t size);
const void *array_get(c_parray_t *array, size_t index);
size_t array_length(c_parray_t *array);
const void *array_insert(c_parray_t *array, size_t index, const void *item, size_t size);
void *array_pop(c_parray_t *array, size_t index);

#endif
