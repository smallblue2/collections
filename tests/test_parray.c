#include "../include/collections/parray.h"
#include "unity/src/unity.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_parray_create() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  parray_free(parray);
}

void test_parray_get() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  int i = 1337;
  int *ret = (int *)parray_append(parray, &i, sizeof(i));
  TEST_ASSERT_NOT_NULL(ret);
  TEST_ASSERT_TRUE(*ret == i);
  int *got = (int *)parray_get(parray, 0);
  TEST_ASSERT_NOT_NULL(got);
  TEST_ASSERT_TRUE(*got = *ret);
  TEST_ASSERT_TRUE(got = ret);
  TEST_ASSERT_TRUE(*got = 1337);
  parray_free(parray);
}

void test_parray_append() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  for (int i = 0; i < 10; i++) {
    parray_append(parray, &i, sizeof(i));
  }
  for (int i = 0; i < 10; i++) {
    int *got = (int *)parray_get(parray, i);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_TRUE(*got == i);
  }
  parray_free(parray);
}

void test_parray_length() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  for (int i = 0; i < 10; i++) {
    TEST_ASSERT_NOT_NULL(parray_append(parray, &i, sizeof(i)));
  }
  TEST_ASSERT_TRUE(parray_length(parray) == 10);
  parray_free(parray);
}

void test_parray_insert() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  for (int i = 0; i < 10; i++) {
    TEST_ASSERT_NOT_NULL(parray_append(parray, &i, sizeof(i)));
  }
  const char *msg = "Hello, World!";
  int msg_size = sizeof(char) * (strlen(msg) + 1);
  const char *insert_got = (char *)parray_insert(parray, 3, msg, msg_size);
  TEST_ASSERT_NOT_NULL(insert_got);
  TEST_ASSERT_TRUE(strncmp(msg, insert_got, msg_size) == 0);
  const char *got = (char *)parray_get(parray, 3);
  TEST_ASSERT_TRUE(strncmp(msg, got, msg_size) == 0);
  TEST_ASSERT_TRUE(strncmp("Hello, World!", got, msg_size) == 0);
  parray_free(parray);
}

void test_parray_insert_oob() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  int tmp = 5;
  TEST_ASSERT_NULL(parray_insert(parray, 3, &tmp, sizeof(tmp)));
  parray_free(parray);
}

void test_parray_pop() {
  c_parray_t *parray = parray_create();
  TEST_ASSERT_NOT_NULL(parray);
  for (int i = 0; i < 12; i++)
    TEST_ASSERT_NOT_NULL(parray_append(parray, &i, sizeof(i)));
  for (int i = 0; i < 12; i++) {
    int *got = (int*)parray_pop(parray, 0);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_TRUE(*got == i);
    free(got);
  }
  TEST_ASSERT_TRUE(parray_length(parray) == 0);
  parray_free(parray);
}

void test_parray_pop_oob() {
  c_parray_t *parray = parray_create();
  void *popped = parray_pop(parray, 0);
  TEST_ASSERT_NULL(popped);
  if (popped != NULL) free(popped);
  parray_free(parray);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_parray_create);
  RUN_TEST(test_parray_get);
  RUN_TEST(test_parray_append);
  RUN_TEST(test_parray_length);
  RUN_TEST(test_parray_insert_oob);
  RUN_TEST(test_parray_insert);
  RUN_TEST(test_parray_pop_oob);
  RUN_TEST(test_parray_pop);
  return UNITY_END();
}
