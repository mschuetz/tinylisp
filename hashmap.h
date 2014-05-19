#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdbool.h>

struct hashmap_entry {
  void * key;
  void * value;
  uint32_t hash;
};

#define HASHMAP_HASH(name) uint32_t (*name)(const void * key, const size_t size)
#define HASHMAP_EQUALS(name) bool (*name)(const void * key1, const void * key2, const size_t size)

struct hashmap {
  struct hashmap_entry *entries;
  double load_factor;
  size_t size;
  size_t load;
  HASHMAP_HASH(hash);
  HASHMAP_EQUALS(equals);
};

struct hashmap * hashmap_create(const size_t initial_size,
    const double load_factor,
    HASHMAP_HASH(hash),
    HASHMAP_EQUALS(equals));
struct hashmap * hashmap_create_string_keys(const size_t initial_size,
    const double load_factor);

// returns true on success
bool hashmap_put(struct hashmap * hm, void * key, void * value);
struct hashmap_entry * hashmap_get_entry(const struct hashmap * hm, const void * key);
void * hashmap_get(const struct hashmap * hm, const void * key);
void *  hashmap_remove(const struct hashmap * hm, const void * key);

void hashmap_free_all(const struct hashmap *hm);

#endif /* HASHMAP_H_ */
