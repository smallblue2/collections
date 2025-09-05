#include "unity/src/unity.h"
#include "../../include/collections/vector.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_vector_create() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_TRUE(vector_size(v) == 0);
  TEST_ASSERT_TRUE(vector_capacity(v) != 0);
  vector_free(v);
}

void test_vector_resize_expand() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  int default_int = 13;
  int res0 = vector_resize(v, 10, &default_int);
  TEST_ASSERT_TRUE(res0 == 0);
  TEST_ASSERT_TRUE(vector_size(v) == 10);
  for (int i = 0; i < 10; i++) {
    int out;
    int res1 = vector_get(v, i, &out);
    TEST_ASSERT_TRUE(res1 == 0);
    TEST_ASSERT_TRUE(out == default_int);
  }
  vector_free(v);
}

void test_vector_resize_shrink() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  // First expand
  int default_int = 13;
  int res0 = vector_resize(v, 10, &default_int);
  TEST_ASSERT_TRUE(res0 == 0);
  TEST_ASSERT_TRUE(vector_size(v) == 10);
  for (int i = 0; i < 10; i++) {
    int out;
    int res1 = vector_get(v, i, &out);
    TEST_ASSERT_TRUE(res1 == 0);
    TEST_ASSERT_TRUE(out == default_int);
  }
  // Now shrink
  int res2 = vector_resize(v, 5, NULL);
  TEST_ASSERT_TRUE(res2 == 0);
  int out = 4;
  int res3 = vector_get(v, 5, &out);
  TEST_ASSERT_TRUE(res3 == -1);
  TEST_ASSERT_TRUE(out == 4);
  int res4 = vector_get(v, 4, &out);
  TEST_ASSERT_TRUE(res4 == 0);
  TEST_ASSERT_TRUE(out == 13);
  vector_free(v);
}

void test_vector_get() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  int default_value = 13;
  int res0 = vector_resize(v, 1, &default_value);
  TEST_ASSERT_TRUE(res0 == 0);
  int out = 0;
  int res1 = vector_get(v, 0, &out);
  TEST_ASSERT_TRUE(res1 == 0);
  TEST_ASSERT_TRUE(out == 13);
  vector_free(v);
}

void test_vector_set() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  int default_value = 13;
  int res0 = vector_resize(v, 1, &default_value);
  TEST_ASSERT_TRUE(res0 == 0);
  int out = 0;
  int res1 = vector_get(v, 0, &out);
  TEST_ASSERT_TRUE(res1 == 0);
  TEST_ASSERT_TRUE(out == 13);
  int to_set = 167;
  int res2 = vector_set(v, 0, &to_set);
  TEST_ASSERT_TRUE(res2 == 0);
  int res3 = vector_get(v, 0, &out);
  TEST_ASSERT_TRUE(res3 == 0);
  TEST_ASSERT_TRUE(out == to_set);
  vector_free(v);
}

void test_vector_reserve() {
  c_vector_t *v = vector_create(sizeof(char));
  TEST_ASSERT_NOT_NULL(v);
  int res = vector_reserve(v, 100);
  TEST_ASSERT_TRUE(res == 0);
  TEST_ASSERT_TRUE(vector_capacity(v) == 100);
  vector_free(v);
}

void test_vector_empty() {
  c_vector_t *v = vector_create(sizeof(char*));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_TRUE(vector_empty(v) == true);
  const char *default_val = "Hello world!";
  int res0 = vector_resize(v, 10, &default_val);
  TEST_ASSERT_TRUE(res0 == 0);
  TEST_ASSERT_TRUE(vector_empty(v) == false);
  vector_free(v);
}

void test_vector_insert() {
  c_vector_t *v = vector_create(sizeof(char*));
  TEST_ASSERT_NOT_NULL(v);
  const char *to_insert = "meow";
  int res0 = vector_insert(v, 0, &to_insert);
  TEST_ASSERT_TRUE(res0 == 0);
  char *out;
  int res1 = vector_get(v, 0, &out);
  TEST_ASSERT_TRUE(res1 == 0);
  TEST_ASSERT_TRUE(strncmp(out, to_insert, strlen(to_insert)) == 0);
  const char *to_insert_again = "woof";
  int res2 = vector_insert(v, 0, &to_insert_again);
  TEST_ASSERT_TRUE(res2 == 0);
  char *out_again;
  int res3 = vector_get(v, 0, &out_again);
  TEST_ASSERT_TRUE(res3 == 0);
  TEST_ASSERT_TRUE(strncmp(out_again, to_insert_again, strlen(to_insert_again)) == 0);
  TEST_ASSERT_TRUE(vector_size(v) == 2);
  vector_free(v);
}

void test_vector_remove_out_param() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  int def = 9999;
  int res0 = vector_resize(v, 8, &def);
  TEST_ASSERT_TRUE(res0 == 0);
  for (int i = 0; i < 8; i++) {
    int out;
    int res1 = vector_remove(v, 0, &out);
    TEST_ASSERT_TRUE(res1 == 0);
    TEST_ASSERT_TRUE(out == def);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 0);
  vector_free(v);
}

void test_vector_remove() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  int def = 9999;
  int res0 = vector_resize(v, 8, &def);
  TEST_ASSERT_TRUE(res0 == 0);
  for (int i = 0; i < 8; i++) {
    int res1 = vector_remove(v, 0, NULL);
    TEST_ASSERT_TRUE(res1 == 0);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 0);
  vector_free(v);
}

void test_vector_push_back() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  for (int i = 0; i < 10; i++) {
    int res0 = vector_push_back(v, &i);
    TEST_ASSERT_TRUE(res0 == 0);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 10);
  for (int i = 9; i >= 0; i--) {
    int out;
    int res1 = vector_get(v, i, &out);
    TEST_ASSERT_TRUE(res1 == 0);
    TEST_ASSERT_TRUE(out == i);
  }
  vector_free(v);
}

void test_vector_pop_back_out_param() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  for (int i = 0; i < 10; i++) {
    int res0 = vector_push_back(v, &i);
    TEST_ASSERT_TRUE(res0 == 0);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 10);
  for (int i = 9; i >= 0; i--) {
    int out;
    int res1 = vector_pop_back(v, &out);
    TEST_ASSERT_TRUE(res1 == 0);
    TEST_ASSERT_TRUE(out == i);
  }
  vector_free(v);
}

void test_vector_pop_back() {
  c_vector_t *v = vector_create(sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  for (int i = 0; i < 10; i++) {
    int res0 = vector_push_back(v, &i);
    TEST_ASSERT_TRUE(res0 == 0);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 10);
  for (int i = 9; i >= 0; i--) {
    int res1 = vector_pop_back(v, NULL);
    TEST_ASSERT_TRUE(res1 == 0);
  }
  TEST_ASSERT_TRUE(vector_size(v) == 0);
  vector_free(v);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_vector_create);
  RUN_TEST(test_vector_resize_expand);
  RUN_TEST(test_vector_resize_shrink);
  RUN_TEST(test_vector_get);
  RUN_TEST(test_vector_set);
  RUN_TEST(test_vector_reserve);
  RUN_TEST(test_vector_empty);
  RUN_TEST(test_vector_insert);
  RUN_TEST(test_vector_remove_out_param);
  RUN_TEST(test_vector_remove);
  RUN_TEST(test_vector_push_back);
  RUN_TEST(test_vector_pop_back_out_param);
  RUN_TEST(test_vector_pop_back);
  return UNITY_END();
}
