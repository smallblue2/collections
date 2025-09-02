#ifndef PARRAYH
#define PARRAYH

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Pointer array
 *
 * A heterogenous dynamic data structure which handles its own memory.
 */
typedef struct parray_t c_parray_t;

/**
 *
 * @brief Creates a new pointer array.
 *
 * A dynamic pointer array is created, allocating memory for
 * its internals.
 * The pointer array automatically grows its size as required.
 *
 * @return Pointer to pointer array, or NULL on failure.
 *
 * @note Must free via `parray_free`.
 */
c_parray_t *parray_create();


/**
 *
 * @brief Frees a pointer array.
 *
 * Frees all memory associated with the array.
 *
 * @param parray The pointer array to free.
 */
void parray_free(c_parray_t *parray);

/**
 *
 * @brief Appends an `item` of `size` to `parry`.
 *
 * Appends an `item` of `size` to `parray`.
 * Copies the value of `item` and stores it.
 * Grows the pointer array if needed.
 *
 * @param parray The pointer array to append to.
 * @param item The item whos value to append.
 * @param size The size of the item's value to append.
 * @return NULL on error, or the pointer array's reference of the new value.
 */
const void *parray_append(c_parray_t *parray, const void *item, const size_t size);

/**
 *
 * @brief Retrieves an item from the pointer array.
 *
 * Retrieves the pointer array's copied value at an index.
 *
 * @param parray The pointer array to get from.
 * @param index The index from which to get.
 * @return NULL on error, or the item at the index within the pointer array.
 */
const void *parray_get(c_parray_t *parray, size_t index);

/**
 *
 * @brief Retrieves the length of a pointer array.
 *
 * Retrieves the length of a pointer array.
 *
 * @param parray The pointer array from which to retrieve the length of.
 * @return The length of the pointer array.
 */
size_t parray_length(c_parray_t *array);

/**
 *
 * @brief Inserts an item into the pointer array.
 *
 * Copies the value of `item` and inserts it into `parray`
 * at `index`.
 *
 * @param parray The array to insert into.
 * @param index The index to insert to.
 * @param item The item to copy the value from and insert.
 * @param size The size of the value to copy from `item`.
 * @return NULL on error, or the pointer array's reference of the copied inserted value.
 */
const void *parray_insert(c_parray_t *array, size_t index, const void *item, size_t size);
void *parray_pop(c_parray_t *array, size_t index);

#endif
