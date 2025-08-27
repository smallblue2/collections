#ifndef ARENAH
#define ARENAH

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>
#include <stdalign.h>

typedef struct arena_t alctr_arena_t;

/**
 *
 * @brief Creates a new memory arena.
 *
 * This arena is used for grouping memory lifetimes and simplifying heap memory management.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to arena, or NULL on failure.
 */
alctr_arena_t *arena_create(size_t size);

/**
 *
 * @brief Allocates a new aligned block of memory from an arena allocator.
 *
 * This takes a block of memory from the arena.
 *
 * @param arena Pointer to the arena allocator.
 * @param size Number of bytes to allocate.
 * @param align The alignment of the memory.
 * @return Pointer to the requested block of memory, or `nullptr` on error.
 *
 * @note The lifetime of the returned memory is tied to the arena.
 *       Do not free() it manually.
 */
void *arena_alloc_aligned(alctr_arena_t *arena, size_t size, size_t align);

/**
 *
 * @brief Allocates a new block of memory from an arena allocator.
 *
 * This takes a block of memory from the arena.
 * It aligns it by default to `alignof(max_align_t)`.
 *
 * @param arena Pointer to the arena allocator.
 * @param size Number of bytes to allocate.
 * @return Pointer to the requested block of memory, or `nullptr` on error.
 *
 * @note Due to using max alignment, it might appear
 *       to be out of memory earlier than expected,
 *       particularly for smaller arenas.
 */
void *arena_alloc(alctr_arena_t *arena, size_t size);

/**
 *
 * @brief Frees an arena allocator.
 *
 * Memory allocated by an arena allocator must not be used after its free.
 *
 * @param arena Pointer to the arena allocator being freed.
 */
void arena_free(alctr_arena_t *arena);

/**
 *
 * @brief Resets an arena allocator.
 *
 * @param arena Pointer to the arena being reset.
 * @return Whether the reset was succesful or not.
 *
 * @note Do not use previously assigned memory after a reset.
 */
void arena_reset(alctr_arena_t *arena);

#endif
