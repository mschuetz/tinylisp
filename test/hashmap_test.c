#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "../hashmap.h"
#include "../base.h"

static void fill(struct hashmap* map) {
  hashmap_put(map, "foo", "bar");
  hashmap_put(map, "baz", "quux");
}

/*
static void dump(struct hashmap* map) {
  for (size_t i = 0; i < map->size; i++) {
    char * k = map->entries[i].key;
    char * v = map->entries[i].value;
    fprintf(stderr, "%zu %p -> %p %s -> %s\n", i, k, v, k == NULL ? "" : k, v == NULL ? "" : v);
  }
}
*/
START_TEST (test_create)
  {
    struct hashmap * map = hashmap_create_string_keys(32, 0.75);
    ck_assert_ptr_ne(map, NULL);
    free(map);
  }END_TEST

START_TEST (test_insert_and_get)
  {
    struct hashmap* map = hashmap_create_string_keys(2, 5.0);
    fill(map);
    ck_assert_str_eq(hashmap_get(map, "foo"), "bar");
    ck_assert_str_eq(hashmap_get(map, "baz"), "quux");
    free(map);
  }END_TEST

START_TEST (test_resize)
  {
    struct hashmap* map = hashmap_create_string_keys(8, 0.75);
    for (int i=0; i<32; i++) {
      char * key;
      char * value;
      asprintf(&key, "key%d", i);
      asprintf(&value, "value%d", i);
      hashmap_put(map, key, value);
    }
    //dump(map);
    for (int i=0; i<32; i++) {
      char * key;
      char * value;
      asprintf(&key, "key%d", i);
      asprintf(&value, "value%d", i);
      char * actual = hashmap_get(map, key);
      ck_assert_ptr_ne(actual, NULL);
      ck_assert_str_eq(actual, value);
      free(key);
      free(value);
    }
    hashmap_free_all(map);
    free(map);
  }END_TEST

START_TEST (test_remove)
  {
    struct hashmap* map = hashmap_create_string_keys(2, 5.0);
    fill(map);
    hashmap_remove(map, "baz");

    ck_assert_str_eq(hashmap_get(map, "foo"), "bar");
    ck_assert_ptr_eq(hashmap_get(map, "baz"), NULL);
    free(map);
  }END_TEST

int main() {
  Suite * s = suite_create("hashmap");
  TCase * tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_create);
  tcase_add_test(tc_core, test_insert_and_get);
  tcase_add_test(tc_core, test_remove);
  tcase_add_test(tc_core, test_resize);
  suite_add_tcase(s, tc_core);
  SRunner * sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  const int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
