#ifndef HASHT_H
#define HASHT_H
// TODO: implement adding and getting items from hash variable
// TODO: see p147 in the C book for reference
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
  char l;
  char r;
} pair;

typedef struct {
  void *key;
  size_t key_len;
  int value;
} ht_item;

typedef struct {
  size_t len;
  uint64_t (*hash_function)(void *key, size_t _size);
  ht_item *items;
} ht;
// hashing routines
uint64_t fnv1_hash(void *key, size_t _size);
uint64_t fnv1a_hash(void *key, size_t _size);
// hash table routines
void ht_display(ht *table);
ht *ht_create(void);
bool ht_init(ht **table); // trying out double pointer thing, will see if it works
bool ht_insert_item(ht *table, ht_item item);
ht_item ht_get_item(ht *table, void *key, size_t _size);
// bool ht_get_item(ht *table, void *key, size_t _size, ht_item *out);
bool ht_destroy(ht *table);
