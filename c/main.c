#include "hasht.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// TODO: Tokenization:
// * Implement pair counting clearly (pair → frequency).
// * Merge the most frequent pair iteratively.
void print_hex(const char *s);
int main(void) {
  // * ✅ Implement vocabulary hash table clearly (token_sequence → frequency).
  const char *readonly_text = "low lower lowest";
  char *text = strdup(readonly_text);
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
    item.item_type = KEY_TYPE_TOKEN;
    insertion = ht_insert_item(vocab, item);
    if (!insertion) {
      printf("Error inserting: %s\n", split_string);
      exit(1);
    }
  }
  ht_display(vocab);
  ht *pair_counter = ht_create();
  pair_counter->hash_function = fnv1_hash_pair;
  size_t token_size = 1;
  text = strdup(readonly_text);

  // * pair counting
  for (size_t i = 0; i < strlen(text) - 1; i++) {
    token l, r;
    pair p;
    l.data = malloc(sizeof(char) * token_size);
    l.data[0] = text[i];
    l.len = token_size;
    r.data = malloc(sizeof(char) * token_size);
    r.data[0] = text[i + 1];
    r.len = token_size;
    p.l = l;
    p.r = r;
    ht_item *found = ht_get_item_pair(pair_counter, &p, sizeof(pair));
    if (found == NULL) {
      ht_item token_pair;
      item_init(&token_pair, &p, sizeof(pair), 1); // should copy not just the pointer addresses, but full l.data and r.data too ideally for full safety.
      token_pair.item_type = KEY_TYPE_PAIR;
      if (!ht_insert_item_pair(pair_counter, token_pair)) {
        printf("Error inserting pair\n");
        exit(1);
      }
    } else {
      found->value += 1;
    }
  }
  ht_display(pair_counter);
  ht_destroy(pair_counter);
  ht_destroy(vocab);

  return 0;
}
