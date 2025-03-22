// TODO: get_item method
#include "hasht.h"
#include <stddef.h>
#include <stdint.h>

uint64_t fnv1_hash(void *key, size_t _size) {
  char *ptr = key;
  uint64_t hash = FNV_OFFSET;
  for (size_t i = 0; i < _size; i++) {
    hash = hash * FNV_PRIME;
    hash = hash ^ (uint64_t)ptr[i];
  }
  return hash;
}
uint64_t fnv1a_hash(void *key, size_t _size) {
  char *ptr = key;
  uint64_t hash = FNV_OFFSET;
  for (size_t i = 0; i < _size; i++) {
    hash = hash ^ (uint64_t)ptr[i];
    hash = hash * FNV_PRIME;
  }
  return hash;
}

ht *ht_create(void) {
  ht *table = malloc(sizeof(ht));
  if (table == NULL) {
    return NULL;
  }
  table->len = INITIAL_SIZE;
  table->items = malloc(sizeof(ht_item) * table->len);
  if (table->items == NULL) {
    free(table->items);
    return NULL;
  }
  table->hash_function = fnv1_hash;
  if (table->hash_function == NULL) {
    printf("NULL in FNV1 function");
    return NULL;
  }
  return table;
}

bool ht_init(ht **table) {
  *table = malloc(sizeof(ht));

  (*table)->len = INITIAL_SIZE;
  (*table)->items = malloc(sizeof(ht_item) * (*table)->len);
  if ((*table)->items == NULL) {
    return false;
  }
  (*table)->hash_function = &fnv1_hash;
  if ((*table)->hash_function == NULL) {
    return false;
  }
  return true;
}
void ht_display(ht *table) {
  for (size_t i = 0; i < table->len; i++) {
    // if (table->items[i].key != NULL) {
    printf("%s\t->\t%d\n", table->items[i].key, table->items[i].value);
    // }
  }
}
bool ht_insert_item(ht *table, ht_item item) {
  uint64_t pos = table->hash_function(item.key, item.key_len) % table->len;
  table->items[pos] = item;
  return 1;
}

ht_item ht_get_item(ht *table, void *key, size_t _size) {
}

void print_hex(const char *s) {
  while (*s)
    printf("%02x", (unsigned int)*s++);
  printf("\t");
}
