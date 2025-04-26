#include "hasht.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_hex(const char *s);
int main(void) {
  pair p;
  char *text = "Test string with many more chars";
  ht *table = ht_create();
  bool insertion;
  // create a table that counts pairs of characters in a string
  for (int i = 0; i < (int)strlen(text) - 1; i++) {
    p.l = text[i];
    p.r = text[i + 1];
    ht_item item;
    item_init(&item, &p, sizeof(p), 0);
    insertion = ht_insert_item(table, item);
    if ((!insertion) || (table->load_factor > 0.75)) {
      printf("LOAD FACTOR %f, resizing\n", table->load_factor);
      ht_resize(table);
      printf("LOAD FACTOR AFTER RESIZE: %f\n", table->load_factor);
    }
  }
  ht_display(table);
  // ht_item get_item = ht_get_item(table, &p, sizeof(p));
  printf("\n\n\n");
  return 0;
}
