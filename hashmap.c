#include "base.h"
#include "hashmap.h"
#include <string.h>

static struct hashmap * allocate() {
  return (struct hashmap *) malloc(sizeof(struct hashmap));
}

static uint32_t djb_hash_str(const void * data, const size_t length) {
  uint32_t hash = 5381;
  char c;
  char * str = (char *) data;
  while ((c = *str++) != 0) {
    hash = ((hash << 5) + hash) + ((uint32_t) c);
  }
  return hash;
}

static bool str_equals(const void * key1, const void * key2, const size_t size) {
  return strcmp(key1, key2) == 0;
}

struct hashmap * hashmap_create(const size_t initial_size,
    const double load_factor,
    uint32_t (*hash)(const void * key, const size_t size),
    bool (*equals)(const void * key1, const void * key2, const size_t size)) {

  struct hashmap * hm = allocate();
  if (hm == NULL)
    return NULL;
  hm->size = initial_size;
  hm->hash = hash;
  hm->equals = equals;
  hm->entries = malloc(initial_size * sizeof(struct hashmap_entry));
  if (hm->entries == NULL) {
    free(hm);
    return NULL;
  }
  hm->load = 0;
  hm->load_factor = load_factor;
  return hm;

}
struct hashmap * hashmap_create_string_keys(size_t initial_size,
    double load_factor) {
  return hashmap_create(initial_size, load_factor, djb_hash_str, str_equals);
}

static bool hashmap_resize(const struct hashmap * hm, size_t new_size) {
  return false;
}

bool hashmap_put(const struct hashmap * hm, void * key, void * value) {
  uint32_t hash = hm->hash(key, -1);
  if (((double) hm->load) / hm->size > hm->load_factor) {
    if (!hashmap_resize(hm, hm->size * 2))
      return false;
  }
  uint32_t start_index = hash % hm->size;
  uint32_t index = start_index;
  for (; index - start_index < hm->size; index++) {
    if (hm->entries[index % hm->size].key == NULL)
      break;
  }
  struct hashmap_entry * entry = &hm->entries[index % hm->size];
  entry->hash = hash;
  entry->key = key;
  entry->value = value;
  return true;
}

static struct hashmap_entry * hashmap_get_entry(const struct hashmap * hm, const void * key) {
  uint32_t hash = hm->hash(key, -1);
  uint32_t start_index = hash % hm->size;
  for (uint32_t index = start_index; index - start_index < hm->size; index++) {
    if (hm->entries[index % hm->size].key == NULL)
      return NULL;
    if (hm->entries[index % hm->size].hash == hash && hm->equals(hm->entries[index % hm->size].key, key, -1))
      return &hm->entries[index % hm->size];
  }
  return NULL;
}

const void * hashmap_get(const struct hashmap * hm, const void * key) {
  struct hashmap_entry * entry = hashmap_get_entry(hm, key);
  if (entry == NULL)
    return NULL;
  return entry->value;
}

void hashmap_remove(const struct hashmap * hm, const void * key) {
  struct hashmap_entry * entry = hashmap_get_entry(hm, key);
  if (entry == NULL)
    return;
  memset(entry, 0, sizeof(struct hashmap_entry));
}
