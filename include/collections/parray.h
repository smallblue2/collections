#ifndef PARRAYH
#define PARRAYH

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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
 * @param parray_free_func Optional destructor function for child elements to be used during free.
 * @return Pointer to pointer array, or NULL on failure.
 *
 * @note Must free via `parray_free`.
 */
c_parray_t *parray_create(void (*parray_free_func)(void*));


/**
 *
 * @brief Frees a pointer array.
 *
 * Frees all memory associated with the array.
 *
 * @param parray The pointer array to free.
 *
 * @note Frees all remaining children if a destructor function was provided in parray creation.
 */
void parray_free(c_parray_t *parray);

/**
 *
 * @brief Appends a pointer to a pointer array.
 *
 * Appends a `ptr` to `parray`.
 * The pointer array takes ownership of the pointer.
 * Grows the pointer array if needed.
 *
 * @param parray The pointer array to append to.
 * @param ptr The pointer to append.
 * @return 0 on success, -1 on error.
 */
int parray_append(c_parray_t *parray, void *ptr);

/**
 *
 * @brief Retrieves a pointer from the pointer array.
 *
 * Retrieves a pointer from the pointer array at a given index.
 * The pointer array still owns the pointer.
 *
 * @param parray The pointer array to get from.
 * @param index The index from which to get.
 * @return NULL on error, or the item at the index within the pointer array.
 */
const void *parray_get(const c_parray_t *parray, size_t index);

/**
 *
 * @brief Retrieves the length of a pointer array.
 *
 * Retrieves the length of a pointer array.
 *
 * @param parray The pointer array from which to retrieve the length of.
 * @return The length of the pointer array.
 */
size_t parray_length(const c_parray_t *parray);

/**
 *
 * @brief Inserts a pointer into the pointer array.
 *
 * Inserts a pointer into the pointer array.
 * The pointer array takes ownership of the pointer.
 *
 * @param parray The array to insert into.
 * @param index The index to insert to.
 * @param ptr The pointer to insert.
 * @return 0 on success, -1 on error.
 */
int parray_insert(c_parray_t *parray, size_t index, void *ptr);

/**
 *
 * @brief Pops a pointer from the pointer array.
 *
 * Pops a pointer from the pointer array, returning
 * ownership to the user.
 *
 * @param parray The pointer array to pop from.
 * @param index The index to pop from.
 * @return NULL on error, or the removed pointer.
 *
 * @note You must free the returned pointer, as the array no longer owns it.
 */
void *parray_pop(c_parray_t *parray, size_t index);

#endif
