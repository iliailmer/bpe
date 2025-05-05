#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hasht.h"
// TODO: Tokenization:
// * Implement pair counting clearly (pair → frequency).
// * Merge the most frequent pair iteratively.
void print_hex(const char *s);
int main(void)
{
  // * ✅ Implement vocabulary hash table clearly (token_sequence → frequency).
  const char *readonly_text = "low lower lowest";
  char *text = strdup(readonly_text);
  ht *vocab = ht_create();
  bool insertion;
  char *split_string = strtok(text, " ");
  while (split_string != NULL) {
    size_t n = strlen(split_string);
    for (size_t i = 0; i < n; i++) {
      token *t = malloc(sizeof(token));
      t->data = malloc(sizeof(char));
      t->data[0] = split_string[i];
      t->len = 1;

      ht_item item;
      item_init(&item, t, sizeof(token), 1, KEY_TYPE_TOKEN);
      ht_item *found = ht_get_item(vocab, t, sizeof(token), KEY_TYPE_TOKEN);
      if (found != NULL) {
        found->value = found->value + 1;
      }
      else {
        insertion = ht_insert_item(vocab, item);
        if (!insertion) {
          printf("Error inserting: %s\n", split_string);
          exit(1);
        }
      }
    }
    split_string = strtok(NULL, " ");
  }
  ht_display(vocab);
  ht *pair_counter = ht_create();
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
    ht_item *found = ht_get_item(pair_counter, &p, sizeof(pair), KEY_TYPE_PAIR);
    if (found == NULL) {
      ht_item token_pair;
      item_init(
          &token_pair, &p, sizeof(pair), 1,
          KEY_TYPE_PAIR); // should copy not just the pointer addresses, but
                          // full l.data and r.data too ideally for full safety.
      if (!ht_insert_item(pair_counter, token_pair)) {
        printf("Error inserting pair\n");
        exit(1);
      }
    }
    else {
      found->value += 1;
    }
  }
  ht_display(pair_counter);
  ht_destroy(pair_counter);
  ht_destroy(vocab);
  return 0;
}
