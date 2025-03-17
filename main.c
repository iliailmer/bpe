#include "hasht.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_hex(const char *s);
int main(void) {
    char *text = "This is a sample text that I wrote while watching Tsoding on Twitch";
    char pair[3] = {0};
    uint64_t hash;
    int text_size = strlen(text);
    for (int i = 0; i < text_size - 1; i++) {
        pair[0] = text[i];
        pair[1] = text[i + 1];
        pair[2] = '\0';
        hash = fnv1_hash(pair);
        print_hex(pair);
        printf("%s:\t\t%llx\n", pair, hash);
    }
    return 0;
}

void print_hex(const char *s) {
    while (*s)
        printf("%02x", (unsigned int)*s++);
    printf("\t");
}
