#include "hasht.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
uint64_t fnv1_hash(void *key, size_t _size)
{
  char *ptr = key;
  uint64_t hash = FNV_OFFSET;
  for (size_t i = 0; i < _size; i++) {
    hash = hash * FNV_PRIME;
    hash = hash ^ (uint64_t)ptr[i];
  }
  return hash;
}

uint64_t fnv1a_hash(void *key, size_t _size)
{
  char *ptr = key;
  uint64_t hash = FNV_OFFSET;
  for (size_t i = 0; i < _size; i++) {
    hash = hash ^ (uint64_t)ptr[i];
    hash = hash * FNV_PRIME;
  }
  return hash;
}

uint64_t fnv1a_hash_tok(void *key)
{
  uint64_t hash = FNV_OFFSET;
  token *p = (token *)key;
  for (size_t i = 0; i < p->len; i++) {
    hash ^= (uint64_t)p->data[i];
    hash *= FNV_PRIME;
  }
  for (size_t i = 0; i < p->len; i++) {
    hash ^= (uint64_t)p->data[i];
    hash *= FNV_PRIME;
  }
  return hash;
}
uint64_t fnv1_hash_tok(void *key)
{
  uint64_t hash = FNV_OFFSET;
  token *p = (token *)key;
  for (size_t i = 0; i < p->len; i++) {
    hash *= (uint64_t)p->data;
    hash ^= FNV_PRIME;
  }
  for (size_t i = 0; i < p->len; i++) {
    hash *= (uint64_t)p->data[i];
    hash ^= FNV_PRIME;
  }
  return hash;
}
uint64_t fnv1a_hash_pair(void *key)
{
  uint64_t hash = FNV_OFFSET;
  pair *p = (pair *)key;
  for (size_t i = 0; i < p->l.len; i++) {
    hash ^= (uint64_t)p->l.data[i];
    hash *= FNV_PRIME;
  }
  for (size_t i = 0; i < p->r.len; i++) {
    hash ^= (uint64_t)p->r.data[i];
    hash *= FNV_PRIME;
  }
  return hash;
}
uint64_t fnv1_hash_pair(void *key)
{
  uint64_t hash = FNV_OFFSET;
  pair *p = (pair *)key;
  for (size_t i = 0; i < p->l.len; i++) {
    hash *= (uint64_t)p->l.data[i];
    hash ^= FNV_PRIME;
  }
  for (size_t i = 0; i < p->r.len; i++) {
    hash *= (uint64_t)p->r.data[i];
    hash ^= FNV_PRIME;
  }
  return hash;
}

size_t hash_item(ht_item item, ht *table)
{
  uint64_t pos;
  switch (item.item_type) {
  case KEY_TYPE_PAIR:
    pos = fnv1_hash_pair(item.key) % table->len;
    break;
  case KEY_TYPE_TOKEN:
    pos = fnv1_hash_tok(item.key) % table->len;
    break;
  case KEY_TYPE_STRING:
    pos = fnv1_hash(item.key, item.key_len);
    break;
  }
  return pos;
}

ht *ht_create(void)
{
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
  table->load_factor = 0.0;
  return table;
}
void ht_destroy(ht *table)
{
  if (!table)
    return;

  for (size_t i = 0; i < table->len; i++) {
    item_destroy(&table->items[i]);
  }

  free(table->items);
  free(table);
}
bool ht_init(ht **table)
{
  *table = malloc(sizeof(ht));

  (*table)->len = INITIAL_SIZE;
  (*table)->items = malloc(sizeof(ht_item) * (*table)->len);
  if ((*table)->items == NULL) {
    return false;
  }
  for (size_t i = 0; i < (*table)->len; i++) {
    (*table)->items[i].occupied = false;
  }
  return true;
}

size_t ht_len(ht table) { return table.len; }

size_t ht_len_2(ht *table) { return table->len; }

void item_display(ht_item item)
{
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

void ht_display(ht *table)
{
  for (size_t i = 0; i < table->len; i++) {
    if (table->items[i].occupied) {
      item_display(table->items[i]);
    }
  }
}

void item_init(ht_item *item, void *key, size_t key_len, uint64_t value,
               key_type_t item_type)
{
  item->key = malloc(key_len);
  memcpy(item->key, key, key_len);
  item->key_len = key_len;
  item->value = value;
  item->occupied = true;
  item->item_type = item_type;
}
void item_destroy(ht_item *item)
{
  if (!item || !item->occupied)
    return;

  if (item->item_type == KEY_TYPE_PAIR) {
    pair *p = (pair *)item->key;
    if (p) {
      if (p->l.data)
        free(p->l.data);
      if (p->r.data)
        free(p->r.data);
      free(p);
    }
  }
  else if (item->item_type == KEY_TYPE_STRING ||
           item->item_type == KEY_TYPE_TOKEN) {
    if (item->key)
      free(item->key);
  }

  item->occupied = false;
  item->key_len = 0;
  item->value = 0;
}

bool token_eq(token *t1, token *t2)
{
  if (t1->len != t2->len)
    return false;
  return memcmp(t1->data, t2->data, t1->len) == 0;
}

bool string_eq(ht_item item1, ht_item item2)
{
  return strcmp(item1.key, item2.key);
}

bool pair_eq(pair *p1, pair *p2)
{
  // Compare left tokens
  if (p1->l.len != p2->l.len)
    return false;
  if (memcmp(p1->l.data, p2->l.data, p1->l.len) != 0)
    return false;

  // Compare right tokens
  if (p1->r.len != p2->r.len)
    return false;
  if (memcmp(p1->r.data, p2->r.data, p1->r.len) != 0)
    return false;

  return true;
}

bool ht_insert_item(ht *table, ht_item item)
{
  uint64_t pos = hash_item(item, table);
  size_t start_pos = pos;
  while (true) {
    ht_item *table_item = &table->items[pos];

    if (!table_item->occupied) {
      table->items[pos] = item;
      table->items[pos].occupied = true;
      table->load_factor = table->load_factor + 1.0 / table->len;
      return 1;
    }
    else {
      if (item.key_len == table_item->key_len) {
        switch (item.item_type) {
        case KEY_TYPE_PAIR:
          if (pair_eq((pair *)item.key, (pair *)table_item->key)) {
            return 1; // key already inserted, not real collision
          }
          break;
        case KEY_TYPE_STRING:
          if (string_eq(item, *table_item)) {
            return 1;
          }
          break;
        case KEY_TYPE_TOKEN:
          if (token_eq((token *)item.key, (token *)table_item->key)) {
            return 1;
          }
          break;
        }
      }
      pos = (pos + 1) % table->len;
      if (pos == start_pos) {
        break;
      }
    }
    if (table->load_factor > 0.7) {
      ht_resize(table);
      pos = hash_item(item, table);
      start_pos = pos;
    }
  }
  return 0;
}

bool ht_resize(ht *table)
{
  ht_item *old_items = table->items;
  size_t old_len = table->len;
  size_t new_len = old_len * 2;
  ht_item *new_items = calloc(new_len, sizeof(ht_item));
  table->items = new_items;
  table->load_factor = 0.0;
  table->len = new_len;
  if (!new_items) {
    return 0;
  }
  for (size_t i = 0; i < old_len; i++) {
    ht_item item = old_items[i];
    if (item.occupied) {
      ht_insert_item(table, item);
    }
  }
  free(old_items);
  return 1;
}

ht_item *ht_get_item(ht *table, void *key, size_t _size, key_type_t item_type)
{
  ht_item tmp;
  item_init(&tmp, key, _size, 0, item_type);
  uint64_t pos = hash_item(tmp, table);
  ht_item table_item = table->items[pos];
  if (!table_item.occupied) {
    return NULL;
  }
  if (_size == table_item.key_len) {

    switch (table_item.item_type) {
    case KEY_TYPE_PAIR:
      if (pair_eq((pair *)key, (pair *)table_item.key)) {
        return &table->items[pos];
        // key already inserted, not real collision
      }
      break;
    case KEY_TYPE_STRING:
      if (strcmp((char *)key, (char *)table_item.key)) {
        return &table->items[pos];
      }
      break;
    case KEY_TYPE_TOKEN:
      if (token_eq((token *)table_item.key, (token *)key)) {
        return &table->items[pos];
      }
      break;
    }
  }
  else {
    size_t start_pos = pos;
    size_t i = (pos + 1) % table->len;
    while (i != start_pos) {
      ht_item table_item = table->items[i];
      if (_size == table_item.key_len) {
        switch (table_item.item_type) {
        case KEY_TYPE_PAIR:
          if (pair_eq((pair *)key, (pair *)table_item.key)) {
            return &table->items[i];
            // key already inserted, not real collision
          }
          break;
        case KEY_TYPE_STRING:
          if (strcmp((char *)key, (char *)table_item.key)) {
            return &table->items[pos];
          }
          break;
        case KEY_TYPE_TOKEN:
          if (token_eq((token *)table_item.key, (token *)key)) {
            return &table->items[pos];
          }
          break;
        }
        i = i + 1;
        i = i % table->len;
      }
    }
  }
  return NULL;
}

void deep_copy(ht_item *item_src, ht_item *item_trg)
{
  item_trg->item_type = item_src->item_type;
  item_trg->value = item_src->value;
  item_trg->occupied = true;
  item_trg->key_len = item_src->key_len;
  if (item_src->item_type == KEY_TYPE_PAIR) {
    pair *orig = (pair *)item_src->key;
    pair *copy = malloc(sizeof(pair));
    copy->l.len = orig->l.len;
    copy->r.len = orig->r.len;
    copy->l.data = malloc(copy->l.len);
    copy->r.data = malloc(copy->r.len);
    memcpy(copy->l.data, orig->l.data, copy->l.len);
    memcpy(copy->r.data, orig->r.data, copy->r.len);
    item_trg->key = copy;
  }
  else if (item_src->item_type == KEY_TYPE_TOKEN) {
    token *orig = (token *)item_src->key;
    token *copy = malloc(sizeof(token));
    copy->len = orig->len;
    copy->data = malloc(copy->len);
    memcpy(copy->data, orig->data, copy->len);
    item_trg->key = copy;
  }
  else { // KEY_TYPE_STRING
    item_trg->key = malloc(item_src->key_len + 1);
    memcpy(item_trg->key, item_src->key, item_src->key_len);
    ((char *)item_trg->key)[item_src->key_len] = '\0'; // null-terminate
  }
}

void item_free(ht_item *item)
{
  if (!item || !item->occupied)
    return;

  pair *p = (pair *)item->key;
  if (p) {
    if (p->l.data)
      free(p->l.data);
    if (p->r.data)
      free(p->r.data);
    free(p);
  }

  item->occupied = false;
}

/**
 * @brief the Get_Item function specifically for token pairs
 *
 * @param table
 * @param key
 * @param _size
 * @return
 */
void print_hex(const char *s)
{
  while (*s)
    printf("%02x", (unsigned int)*s++);
  printf("\t");
}
