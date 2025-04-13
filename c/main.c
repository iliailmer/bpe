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

  // create a table that counts pairs of characters in a string
  for (int i = 0; (int)strlen(text) - 2 > i; i++) {
    p.l = text[i];
    p.r = text[i + 1];
    ht_item item;
    item_init(&item, &p, sizeof(p), 0);
    ht_insert_item(table, item);
  }
  ht_display(table);
  // ht_item get_item = ht_get_item(table, &p, sizeof(p));
  printf("\n\n\n");
  // item_display(get_item);
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
