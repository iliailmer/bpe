#include "hasht.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO: Tokenization:
// • Implement pair counting clearly (pair → frequency).
// • Merge the most frequent pair iteratively.
void print_hex(const char *s);
int main(void) {
  // • ✅ Implement vocabulary hash table clearly (token_sequence → frequency).
  char text[] = "low lower lowest";
  ht *vocab = ht_create();
  bool insertion;
  char *split_string = strtok(text, " ");
  while (split_string != NULL) {
    size_t n = strlen(split_string);
    token *tokens = malloc(sizeof(token) * n);
    for (size_t i = 0; i < n; i++) {
      tokens[i].data = malloc(sizeof(char) * 1);
      tokens[i].data[0] = split_string[i];
      tokens[i].len = 1;
    }
    split_string = strtok(NULL, " ");
    ht_item item;
    item_init(&item, tokens, sizeof(token) * n, 1);
    insertion = ht_insert_item(vocab, item);
    if (!insertion) {
      printf("Error inserting: %s\n", split_string);
      exit(1);
    }
  }
  ht_display(vocab);
  return 0;
  // create a table that counts pairs of characters in a string
  // for (int i = 0; i < (int)strlen(text) - 1; i++) {
  //   p.l = text[i];
  //   p.r = text[i + 1];
  //   ht_item item;
  //   item_init(&item, &p, sizeof(p), 0);
  //   insertion = ht_insert_item(table, item);
  //   if ((!insertion) || (table->load_factor > 0.75)) {
  //     // printf("LOAD FACTOR %f, resizing\n", table->load_factor);
  //     ht_resize(table);
  //     // printf("LOAD FACTOR AFTER RESIZE: %f\n", table->load_factor);
  //   }
  // }
  // ht_display(table);
  // printf("\n\n\n");
  // p.l = 'n';
  // p.r = 'y';
  // ht_item get_item = *(ht_get_item(table, &p, sizeof(p)));
  // item_display(get_item);
  // printf("\n\n\n");
  // return 0;
}
