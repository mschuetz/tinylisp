#ifndef HASHMAP_H_
#define HASHMAP_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  void * key;
  void * value;
  uint32_t hash;
} hashmap_entry;

typedef uint32_t (*hashmap_hash_fn)(const void * key, const size_t size);
typedef bool (*hashmap_equals_fn)(const void * key1, const void * key2, const size_t size);

typedef struct {
  hashmap_entry *entries;
  double load_factor;
  size_t size;
  size_t load;
  hashmap_hash_fn hash;
  hashmap_equals_fn equals;
} hashmap;

hashmap * hashmap_create(const size_t initial_size, const double load_factor, hashmap_hash_fn hash, hashmap_equals_fn equals);
hashmap * hashmap_create_string_keys(const size_t initial_size, const double load_factor);

// returns true on success
bool hashmap_put(hashmap * hm, void * key, void * value);
void * hashmap_get(const hashmap * hm, const void * key);
void *  hashmap_remove(const hashmap * hm, const void * key);

void hashmap_free_all(const hashmap *hm);

#endif /* HASHMAP_H_ */
