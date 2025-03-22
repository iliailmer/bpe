#include "hasht.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_hex(const char *s);
int main(void) {
  pair p;
  char *text = "This is a sample text that I wrote while watching Tsoding on Twitch";
  ht *table = ht_create();
  p.l = text[0];
  p.r = text[1];
  ht_item item;
  item.key = malloc(sizeof(pair));
  memcpy(item.key, &p, sizeof(p));
  item.key_len = sizeof(pair);
  item.value = 0;
  ht_insert_item(table, item);
  ht_display(table);
  // uint64_t hash;
  // int text_size = strlen(text);
  // for (int i = 0; i < text_size - 1; i++) {
  //   pair[0] = text[i];
  //   pair[1] = text[i + 1];
  //   pair[2] = '\0';
  //   hash = fnv1_hash(pair);
  //   print_hex(pair);
  //   printf("%s:\t\t%llx\n", pair, hash);
  // }
  return 0;
}
