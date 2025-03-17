#ifndef HASHT_H
#define HASHT_H
// TODO: implement adding and getting items from hash variable
// TODO: see p147 in the C book for reference
#pragma once
#endif

#define FNV_PRIME 1099511628211UL
#define FNV_OFFSET 14695981039346656037UL

#include "stdint.h"
typedef struct {
    int len;

} ht;

uint64_t fnv1_hash(void *key);
uint64_t fnv1a_hash(void *key);
