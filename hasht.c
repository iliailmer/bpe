#include "hasht.h"
#include <stdint.h>
#include <stdio.h>

uint64_t fnv1_hash(void *key) {
    char *ptr;
    uint64_t hash;
    unsigned char byte;

    ptr = key;
    hash = FNV_OFFSET;
    byte = 0;
    while (*ptr != '\0') {
        printf("%c   ", *ptr);
        byte = (unsigned char)*ptr;
        hash = hash * FNV_PRIME;
        hash = hash ^ (uint64_t)byte;
        ptr++;
    }
    return hash;
}
uint64_t fnv1a_hash(void *key) {
    char *ptr;
    uint64_t hash;
    unsigned char byte;

    ptr = key;
    hash = FNV_OFFSET;
    byte = 0;
    while (*ptr != '\0') {
        printf("%c   ", *ptr);
        byte = (unsigned char)*ptr;
        hash = hash ^ (uint64_t)byte;
        hash = hash * FNV_PRIME;
        ptr++;
    }
    return hash;
}
