#include "caesar.h"
#include <ctype.h>
#include <string.h>

void caesar_cipher(const char *text, int shift, char *output) {
    char ch;
    for (int i = 0; text[i] != '\0'; ++i) {
        ch = text[i];

        if (isalnum(ch)) {
            if (islower(ch)) {
                ch = (ch - 'a' + shift) % 26 + 'a';
            }
            if (isupper(ch)) {
                ch = (ch - 'A' + shift) % 26 + 'A';
            }
            if (isdigit(ch)) {
                ch = (ch - '0' + shift) % 10 + '0';
            }
        } else {
            output[0] = '\0'; // Mensagem inválida
            return;
        }

        output[i] = ch;
    }
    output[strlen(text)] = '\0';
}
