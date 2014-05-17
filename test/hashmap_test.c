#include <check.h>
#include <stdlib.h>
#include "../hashmap.h"

static void fill(struct hashmap* map) {
  hashmap_put(map, "foo", "bar");
  hashmap_put(map, "baz", "quux");
}

START_TEST (test_create)
  {
    struct hashmap * map = hashmap_create_string_keys(32, 0.75);
    ck_assert_ptr_ne(map, NULL);
  }END_TEST

START_TEST (test_insert_and_get)
  {
    struct hashmap* map = hashmap_create_string_keys(2, 5.0);
    fill(map);
    ck_assert_str_eq(hashmap_get(map, "foo"), "bar");
    ck_assert_str_eq(hashmap_get(map, "baz"), "quux");
  }END_TEST

START_TEST (test_remove)
  {
    struct hashmap* map = hashmap_create_string_keys(2, 5.0);
    fill(map);
    hashmap_remove(map, "baz");

    ck_assert_str_eq(hashmap_get(map, "foo"), "bar");
    ck_assert_ptr_eq(hashmap_get(map, "baz"), NULL);
  }END_TEST

int main() {
  Suite * s = suite_create("hashmap");
  TCase * tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_create);
  tcase_add_test(tc_core, test_remove);
  tcase_add_test(tc_core, test_insert_and_get);
  suite_add_tcase(s, tc_core);
  SRunner * sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  const int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
