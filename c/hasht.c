#include "hasht.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
  for (int i = 0; i < (int)table->len; i++) {
    table->items[i].occupied = false;
  }
  table->hash_function = fnv1_hash;
  if (table->hash_function == NULL) {
    printf("NULL in FNV1 function");
    return NULL;
  }
  table->load_factor = 0.0;
  return table;
}

bool ht_init(ht **table) {
  *table = malloc(sizeof(ht));

  (*table)->len = INITIAL_SIZE;
  (*table)->items = malloc(sizeof(ht_item) * (*table)->len);
  if ((*table)->items == NULL) {
    return false;
  }
  for (size_t i = 0; i < (*table)->len; i++) {
    (*table)->items[i].occupied = false;
  }
  (*table)->hash_function = &fnv1_hash;
  if ((*table)->hash_function == NULL) {
    return false;
  }
  return true;
}

size_t ht_len(ht table) {
  return table.len;
}

size_t ht_len_2(ht *table) {
  return table->len;
}

void item_display(ht_item item) {
  if (!item.occupied)
    return; // skip empty slots
  token *tokens = (token *)item.key;
  size_t token_count = item.key_len / sizeof(token);
  // Print key
  for (size_t i = 0; i < token_count; i++) {
    printf("'");
    for (size_t j = 0; j < tokens[i].len; j++) {
      printf("%c", (tokens[i].data[j]));
    }
    printf("' ");
  }

  // Print value
  printf("\t->\t%d\n", item.value);
}

void ht_display(ht *table) {
  for (size_t i = 0; i < table->len; i++) {
    if (table->items[i].occupied) {
      item_display(table->items[i]);
    }
  }
}

void item_init(ht_item *item, void *key, size_t key_len, uint64_t value) {
  item->key = malloc(key_len);
  memcpy(item->key, key, key_len);
  item->key_len = key_len;
  item->value = value;
  item->occupied = true;
}

bool ht_insert_item(ht *table, ht_item item) {
  uint64_t pos = table->hash_function(item.key, item.key_len) % table->len;
  if (table->items[pos].occupied == false) {
    // printf("SUCCESS\n");
    table->items[pos] = item;
    table->items[pos].occupied = true;
    table->load_factor = table->load_factor + 1.0 / table->len;
    return 1;
  } else {
    if (item.key_len == table->items[pos].key_len &&
        memcmp(table->items[pos].key, item.key, item.key_len) == 0) {
      // printf("DUPLICATE: %c\n", *(char *)(item.key));
      return 1; // key already inserted, not real collision
    }
    // printf("COLLISION: %c\n", *(char *)(item.key));
    size_t start_pos = pos;
    size_t i = (pos + 1) % table->len;
    while (i != start_pos) {
      if (table->items[i].occupied == false) {
        // printf("COLLISION RESOLVED\n");
        table->items[i] = item;
        table->items[i].occupied = true;
        table->load_factor = table->load_factor + 1.0 / table->len;
        return 1;
      } else if (item.key_len == table->items[i].key_len &&
                 memcmp(table->items[i].key, item.key, item.key_len) == 0) {
        // printf("DUPLICATE: %c\n", *(char *)(item.key));
        return 1; // key already inserted, not real collision
      }

      i = i + 1;
      i = i % table->len;
    }
  }
  return 0;
}

bool ht_resize(ht *table) {
  table->load_factor = 0;
  ht_item *new_items = calloc(table->len * 2, sizeof(ht_item));
  size_t new_len = table->len * 2;
  if (!new_items) {
    return 0;
  }
  for (size_t i = 0; i < table->len; i++) {
    ht_item item = table->items[i];
    if (item.occupied) {
      size_t pos = table->hash_function(item.key, item.key_len) % new_len;
      while (new_items[pos].occupied) {
        pos += 1;            // move along
        pos = pos % new_len; // NOTE: bad loop?
      }
      new_items[pos] = item;
      new_items[pos].occupied = true;
      table->load_factor = table->load_factor + 1.0 / new_len;
    }
  }
  free(table->items);
  table->items = new_items;
  table->len = new_len;

  return 0;
}

ht_item *ht_get_item(ht *table, void *key, size_t _size) {
  uint64_t pos = table->hash_function(key, _size) % table->len;
  ht_item item = table->items[pos];
  if (!item.occupied) {
    return NULL;
  }
  if (_size == item.key_len && memcmp(item.key, key, _size) == 0) {
    return &table->items[pos];
  } else {
    size_t start_pos = pos;
    size_t i = (pos + 1) % table->len;
    while (i != start_pos) {
      ht_item item = table->items[i];
      if (_size == item.key_len && memcmp(item.key, key, _size) == 0) {
        return &table->items[i];
      }
      i = i + 1;
      i = i % table->len;
    }
  }
  return NULL;
}

void print_hex(const char *s) {
  while (*s)
    printf("%02x", (unsigned int)*s++);
  printf("\t");
}
