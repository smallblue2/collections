#include <stdint.h>
#include <string.h>
#include "unity/src/unity.h"
#include "../include/allocators/arena.h"

void setUp(void) {}
void tearDown(void) {}

void test_arena_create() {
  alctr_arena_t *arena = arena_create(1024);
  TEST_ASSERT_NOT_NULL(arena);
  arena_free(arena);
}

void test_arena_alloc_aligned_8() {
  alctr_arena_t *arena = arena_create(678);
  TEST_ASSERT_NOT_NULL(arena);
  void *ptr1 = arena_alloc_aligned(arena, 12, 8);
  TEST_ASSERT_NOT_NULL(ptr1);
  TEST_ASSERT_TRUE((uintptr_t)ptr1 % 8 == 0);
  void *ptr2 = arena_alloc_aligned(arena, 12, 8);
  TEST_ASSERT_NOT_NULL(ptr2);
  TEST_ASSERT_TRUE((uintptr_t)ptr2 % 8 == 0);
  arena_free(arena);
}

void test_arena_alloc_aligned_4() {
  alctr_arena_t *arena = arena_create(54);
  TEST_ASSERT_NOT_NULL(arena);
  void *ptr1 = arena_alloc_aligned(arena, 12, 4);
  TEST_ASSERT_NOT_NULL(ptr1);
  TEST_ASSERT_TRUE((uintptr_t)ptr1 % 4 == 0);
  void *ptr2 = arena_alloc_aligned(arena, 12, 4);
  TEST_ASSERT_NOT_NULL(ptr2);
  TEST_ASSERT_TRUE((uintptr_t)ptr2 % 4 == 0);
  arena_free(arena);
}

void test_arena_alloc() {
  alctr_arena_t *arena = arena_create(78);
  TEST_ASSERT_NOT_NULL(arena);
  void *ptr = arena_alloc(arena, 12);
  TEST_ASSERT_NOT_NULL(ptr);
  arena_free(arena);
}

void test_arena_reset() {
  alctr_arena_t *arena = arena_create(182);
  TEST_ASSERT_NOT_NULL(arena);
  char *msg = arena_alloc(arena, 12);
  strncpy(msg, "hello world", 12);
  TEST_ASSERT_TRUE(strcmp(msg, "hello world") == 0);
  arena_reset(arena);
  TEST_ASSERT_TRUE(strcmp(msg, "hello world") != 0);
  void *ptr = arena_alloc(arena, 12);
  TEST_ASSERT_TRUE((uintptr_t)ptr == (uintptr_t)msg);
  arena_free(arena);
}

void test_fail_when_full() {
  alctr_arena_t *arena = arena_create(sizeof(int) * 4);
  TEST_ASSERT_NOT_NULL(arena);
  int *ptr1 = (int*)arena_alloc(arena, sizeof(int) * 2);
  TEST_ASSERT_NOT_NULL(ptr1);
  int *ptr2 = (int*)arena_alloc(arena, sizeof(int) * 3);
  TEST_ASSERT_NULL(ptr2);
  int *ptr3 = (int*)arena_alloc_aligned(arena, sizeof(int), 4);
  TEST_ASSERT_NOT_NULL(ptr3);
  arena_free(arena);
}

void test_integrity() {
  alctr_arena_t *arena = arena_create(1024);
  char *msg1 = arena_alloc(arena, 12);
  strncpy(msg1, "hello world", 12);
  TEST_ASSERT_TRUE(strncmp(msg1, "hello world", 12) == 0);
  char *msg2 = arena_alloc(arena, 8);
  strncpy(msg2, "goodbye", 8);
  TEST_ASSERT_TRUE(strncmp(msg2, "goodbye", 8) == 0);
  TEST_ASSERT_TRUE(strncmp(msg1, "hello world", 12) == 0);
  arena_free(arena);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_arena_alloc);
  RUN_TEST(test_arena_alloc_aligned_4);
  RUN_TEST(test_arena_alloc_aligned_8);
  RUN_TEST(test_arena_create);
  RUN_TEST(test_arena_reset);
  RUN_TEST(test_fail_when_full);
  RUN_TEST(test_integrity);
  return UNITY_END();
}
