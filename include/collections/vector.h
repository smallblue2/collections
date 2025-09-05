#ifndef VECTORH
#define VECTORH

typedef struct vector_t c_vector_t;

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 *
 * @brief Creates a vector.
 *
 * Creates a vector, a dynamic homogenous array.
 *
 * @param elem_size The size of the elements to be contained by the vector.
 * @return The newly created vector.
 *
 * @note Do not free the vector manually, use `vector_free()`.
 */
c_vector_t *vector_create(size_t elem_size);

/**
 *
 * @brief Frees a vector.
 *
 * Frees a vector created via `vector_free`.
 *
 * @param vector The vector to be freed.
 */
void vector_free(c_vector_t *vector);

/**
 *
 * @brief Sets an existing index to a value in a vector.
 *
 * Sets an existing index to an element in a vector.
 *
 * @param vector The vector in which an index is being set in.
 * @param index The index to set.
 * @param value The item to copy the value of into the index.
 * @return 0 on success, -1 on error.
 *
 * @note Copies the passed item by value.
 */
int vector_set(c_vector_t *vector, size_t index, const void *value);

/**
 *
 * @brief Gets a value at an index in a vector.
 *
 * Gets a value at an index in a vector via out-parameter.
 *
 * @param vector The vector to retrieve from.
 * @param index The index to get from.
 * @param out An out-parameter to fill with the retrieved value.
 * @return 0 on success, -1 on error.
 */
int vector_get(const c_vector_t *vector, size_t index, void *out);

/**
 *
 * @brief Inserts an element at an index within a vector.
 *
 * Inserts an element at an index within a vector.
 * Existing values are pushed forward to make space.
 *
 * @param vector The vector to insert into.
 * @param index The index to insert to.
 * @param value The value to insert.
 * @return 0 on success, -1 on error.
 *
 * @note Index needs to be below or equal to the size of the vector.
 */
int vector_insert(c_vector_t *vector, size_t index, const void *value);

/**
 *
 * @brief Removes an item from a vector.
 *
 * Removes an item from a vector.
 * Existing elements are pushed back to fill the gap.
 *
 * @param vector The vector to remove an element from.
 * @param index The index to remove.
 * @param out Optional parameter to fill with the removed value.
 * @return 0 on success, -1 on error.
 */
int vector_remove(c_vector_t *vector, size_t index, void *out);

/**
 *
 * @brief Pushes a value to the back of a vector.
 *
 * Pushes a value to the back of a vector.
 *
 * @param vector The vector to push into.
 * @param value The value to push.
 * @return 0 on success, -1 on error.
 */
int vector_push_back(c_vector_t *vector, const void *value);

/**
 *
 * @brief Pops the value at the back of a vector.
 *
 * Pops the value at the back of a vector.
 *
 * @param vector The vector to pop from.
 * @param out Optional out-parameter to fill with the popped value.
 */
int vector_pop_back(c_vector_t *vector, void *out);

/**
 *
 * @brief Manually extends a vector's memory.
 *
 * Manually increases a vector's capacity.
 * Can be used to reduce automatic internal resizing during inserts or pushes.
 *
 * @param vector The vector to resize.
 * @param capacity The capacity to resize to.
 *
 * @note Capacity is the number of elements it's ready to store. Size is the current number of elements within the vector.
 */
int vector_reserve(c_vector_t *vector, size_t capacity);

/**
 *
 * @brief Resizes a vector with a default value.
 *
 * Resizes a vector with a default value.
 *
 * @param vector The vector to resize.
 * @param size The size to expand or shrink to.
 * @param default_value The value to add when expanding.
 * @return 0 on success, -1 on error.
 *
 * @note default_value is only required when expanding the size.
 */
int vector_resize(c_vector_t *vector, size_t size, const void *default_value);

/**
 *
 * @brief Retrieves the vector's current size.
 *
 * Retrieves the vector's current size.
 * The size is the current number or elements within the vector.
 *
 * @param vector The vector to retrieve the size of.
 * @return The size of the vector, or 0 on error.
 */
size_t vector_size(const c_vector_t *vector);

/**
 *
 * @brief Retrieves the vector's current capacity.
 *
 * Retrieves the vector's current capacity.
 * The capacity is the current number of elements the vector is able to store.
 * The capacity will be automatically increased when inserting/pushing elements to the vector.
 *
 * @param vector The vector to retrieve the capacity of.
 * @return The capacity of the vector, or 0 on error.
 */
size_t vector_capacity(const c_vector_t *vector);

/**
 *
 * @brief Returns whether a vector is empty or not.
 *
 * Returns whether a vector is empty or not.
 *
 * @param vector The vector being checked for emptiness.
 * @return A boolean value whether the vector is empty, or false on error.
 */
bool vector_empty(const c_vector_t *vector);

#endif
