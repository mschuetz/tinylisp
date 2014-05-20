#include "base.h"
#include "hashmap.h"
#include <string.h>

static struct hashmap * allocate() {
  return (struct hashmap *) malloc(sizeof(struct hashmap));
}

static uint32_t djb_hash_str(const void * data, const size_t size) {
  uint32_t hash = 5381;
  char c;
  char * str = (char *) data;
  if (size == 0) {
    while ((c = *str++) != 0)
      hash = ((hash << 5) + hash) + ((uint32_t) c);
  } else {
    for (size_t i = 0; i < size; i++)
      hash = ((hash << 5) + hash) + ((uint32_t) str[i]);
  }
  return hash;
}

static bool str_equals(const void * key1, const void * key2, const size_t size) {
  if (size > 0)
    return strncmp(key1, key2, size) == 0;
  return strcmp(key1, key2) == 0;
}

struct hashmap * hashmap_create(const size_t initial_size,
    const double load_factor,
    HASHMAP_HASH(hash),
    HASHMAP_EQUALS(equals)) {

  struct hashmap * hm = allocate();
  if (hm == NULL)
    return NULL;
  hm->entries = calloc(initial_size, sizeof(struct hashmap_entry));
  if (hm->entries == NULL) {
    free(hm);
    return NULL;
  }
  hm->size = initial_size;
  hm->hash = hash;
  hm->equals = equals;
  hm->load = 0;
  hm->load_factor = load_factor;
  return hm;

}
struct hashmap * hashmap_create_string_keys(size_t initial_size,
    double load_factor) {
  return hashmap_create(initial_size, load_factor, djb_hash_str, str_equals);
}

static bool put_with_hash_no_resize(struct hashmap * hm, void * key,
    void * value, uint32_t hash) {
  LOG("%p -> %p hash=%d", key, value, hash);
  uint32_t start_index = hash % hm->size;
  struct hashmap_entry * entry = NULL;
  for (uint32_t i = 0; i < hm->size; i++) {
    entry = &hm->entries[(i + start_index) % hm->size];
    if (entry->key == NULL)
      break;
  }
  if (entry == NULL) {
    return false;
  }
  entry->hash = hash;
  entry->key = key;
  entry->value = value;
  hm->load++;
  return true;
}

static bool hashmap_resize(struct hashmap * hm, size_t new_size) {
  LOG("resize %zu", new_size);
  struct hashmap_entry * old_entries = hm->entries;
  struct hashmap_entry * new_entries = calloc(new_size,
      sizeof(struct hashmap_entry));
  if (new_entries == NULL)
    return false;
  hm->entries = new_entries;
  hm->load = 0;
  const size_t old_size = hm->size;
  hm->size = new_size;
  for (size_t i = 0; i < old_size; i++) {
    if (old_entries[i].key == NULL)
      continue;
    put_with_hash_no_resize(hm, old_entries[i].key, old_entries[i].value,
        old_entries[i].hash);
  }
  return true;
}

bool hashmap_put(struct hashmap * hm, void * key, void * value) {
  LOG("%p -> %p", key, value);
  if ((double) hm->load / (double) hm->size > hm->load_factor) {
    if (!hashmap_resize(hm, hm->size * 2))
      return false;
  }
  uint32_t hash = hm->hash(key, 0);
  return put_with_hash_no_resize(hm, key, value, hash);
}

struct hashmap_entry * hashmap_get_entry(const struct hashmap * hm,
    const void * key) {
  uint32_t hash = hm->hash(key, 0);
  uint32_t start_index = hash % hm->size;
  for (uint32_t i = 0; i < hm->size; i++) {
    struct hashmap_entry * entry = &hm->entries[(i + start_index) % hm->size];
    if (entry->key == NULL)
      return NULL;
    if (entry->hash == hash && hm->equals(entry->key, key, 0))
      return entry;
  }
  return NULL;
}

void * hashmap_get(const struct hashmap * hm, const void * key) {
  struct hashmap_entry * entry = hashmap_get_entry(hm, key);
  if (entry == NULL)
    return NULL;
  return entry->value;
}

void * hashmap_remove(const struct hashmap * hm, const void * key) {
  struct hashmap_entry * entry = hashmap_get_entry(hm, key);
  if (entry == NULL)
    return NULL;
  void * value = entry->value;
  memset(entry, 0, sizeof(struct hashmap_entry));
  return value;
}

void hashmap_free_all(const struct hashmap *hm) {
  for (size_t i = 0; i < hm->size; i++) {
    struct hashmap_entry * entry = &hm->entries[i];
    if (entry->key == NULL)
      continue;
    free(entry->key);
    free(entry->value);
    memset(entry, 0, sizeof(struct hashmap_entry));
  }
}
