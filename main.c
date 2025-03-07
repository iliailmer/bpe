#include "hasht.h"
#include <stdio.h>
#include <string.h>

int main() {
    const char *text = "This is a sample text that I wrote while watching Tsoding on Twitch";
    int text_size = strlen(text);
    for (int i = 0; i < text_size; i++) {
        char a = text[i];
        char b = text[i + 1];
    }
    return 0;
}
