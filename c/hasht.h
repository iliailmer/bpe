#ifndef HASHT_H
#define HASHT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once
#endif

#define FNV_PRIME 1099511628211UL
#define FNV_OFFSET 14695981039346656037UL
#define INITIAL_SIZE 16

typedef struct {
  char *data;
  size_t len;
} token;

typedef struct {
  token l;
  token r;
} pair;

typedef enum { KEY_TYPE_PAIR, KEY_TYPE_TOKEN, KEY_TYPE_STRING } key_type_t;

typedef struct {
  void *key;
  size_t key_len;
  int value;
  bool occupied;
  key_type_t item_type;
} ht_item;
// thisoasdhf;aisjdhfnalkshfl

typedef struct {
  size_t len;
  // uint64_t (*hash_function)(void *key, size_t _size);
  ht_item *items;
  float load_factor;
} ht;

// item routines
void item_display(ht_item item);
void item_init(ht_item *item, void *key, size_t key_len, uint64_t value,
               key_type_t item_type);
void item_destroy(ht_item *item);

void deep_copy(ht_item *item_src, ht_item *item_trg);
// hashing routines
uint64_t fnv1_hash(void *key, size_t _size);
uint64_t fnv1a_hash(void *key, size_t _size);

uint64_t fnv1_hash_tok(void *key);
uint64_t fnv1a_hash_tok(void *key);

uint64_t fnv1_hash_pair(void *key);
uint64_t fnv1a_hash_pair(void *key);

// hash table routines
void ht_display(ht *table);
ht *ht_create(void);
void ht_destroy(ht *table);
bool ht_resize(ht *table);
bool ht_init(
    ht **table); // trying out double pointer thing, will see if it works
bool ht_insert_item(ht *table, ht_item item);
ht_item *ht_get_item(ht *table, void *key, size_t _size, key_type_t item_type);
size_t ht_len(ht table);

// bool ht_get_item(ht *table, void *key, size_t _size, ht_item *out);
