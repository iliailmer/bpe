#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hasht.h"
void print_hex(const char *s);
int main(void)
{
  const char *readonly_text = "this is a big text i like it";
  char *text = strdup(readonly_text);
  ht *vocab = ht_create();
  size_t len = strlen(text);
  token *tokens = malloc(len * sizeof(token));
  size_t token_len = 0;
  for (size_t i = 0; i < len; i++) {
    tokens[token_len].len = 1;
    tokens[token_len].data = malloc(1);
    tokens[token_len].data[0] = text[i];
    token_len++;
  }
  for (size_t i = 0; i < token_len; i++) {
    token *t = &tokens[i];
    ht_item *found = ht_get_item(vocab, t, sizeof(token), KEY_TYPE_TOKEN);
    if (found != NULL) {
      found->value += 1;
    }
    else {
      ht_item item;
      item_init(&item, t, sizeof(token), 1, KEY_TYPE_TOKEN);
      ht_insert_item(vocab, item);
    }
  }
  ht_display(vocab);
  int max_iter = 100;
  int current_iter = 0;
  while (current_iter < max_iter) {
    printf("Current Iteration: %d\n", current_iter + 1);
    current_iter += 1;
    int max_value = -1;
    ht_item max_pair;
    ht *pair_counter = ht_create();
    for (size_t i = 0; i < token_len - 1; i++) {
      pair p = {tokens[i], tokens[i + 1]};
      ht_item *found =
          ht_get_item(pair_counter, &p, sizeof(pair), KEY_TYPE_PAIR);
      if (found != NULL) {
        found->value += 1;
      }
      else {
        ht_item item;
        item_init(&item, &p, sizeof(pair), 1, KEY_TYPE_PAIR);
        ht_insert_item(pair_counter, item);
      }
    }
    // Step 3b: Find most frequent pair
    for (size_t i = 0; i < pair_counter->len; i++) {
      ht_item *it = &pair_counter->items[i];
      if (it->occupied && it->value > max_value) {
        max_value = it->value;
        max_pair = *it;
      }
    }
    if (max_value < 1) {
      printf("No more merges\n");
      ht_destroy(pair_counter);
      break;
    }
    // Step 3c: Create merged token
    pair *max_pair_key = max_pair.key;
    token new_token;
    new_token.len = max_pair_key->l.len + max_pair_key->r.len;
    new_token.data = malloc(new_token.len);

    memcpy(new_token.data, max_pair_key->l.data, max_pair_key->l.len);
    memcpy(new_token.data + max_pair_key->l.len, max_pair_key->r.data,
           max_pair_key->r.len);

    // Step 3d: Replace (l, r) with merged token in tokens[]
    size_t new_len = 0;

    token *new_tokens = malloc(token_len * sizeof(token)); // max same size
    for (size_t i = 0; i < token_len;) {
      if ((i < token_len - 1) && token_eq(&tokens[i], &max_pair_key->l) &&
          token_eq(&tokens[i + 1], &max_pair_key->r)) {
        new_tokens[new_len].len = new_token.len;
        new_tokens[new_len].data = malloc(new_token.len);
        memcpy(new_tokens[new_len].data, new_token.data, new_token.len);
        new_len += 1;
        i += 2;
      }
      else {
        new_tokens[new_len].len = tokens[i].len;
        new_tokens[new_len].data = malloc(tokens[i].len);
        memcpy(new_tokens[new_len].data, tokens[i].data, tokens[i].len);
        new_len += 1;
        i += 1;
      }
    }
    // Cleanup
    for (size_t i = 0; i < token_len; i++) {
      free(tokens[i].data);
    }
    free(tokens);
    ht_destroy(pair_counter);

    tokens = new_tokens;
    token_len = new_len;

    // Step 3e: Insert new token into vocab
    ht_item *found =
        ht_get_item(vocab, &new_token, sizeof(token), KEY_TYPE_TOKEN);
    if (found != NULL) {
      found->value += 1;
    }
    else {
      ht_item item;
      item_init(&item, &new_token, sizeof(token), 1, KEY_TYPE_TOKEN);
      ht_insert_item(vocab, item);
    }

    free(new_token.data);
  }
  printf("*** FINAL VOCAB ***\n");
  ht_display(vocab);
  return 0;
}
