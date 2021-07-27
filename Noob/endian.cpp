#include <stdio.h>

int main() {
    unsigned int x = 0x12345678;
    char *c = (char *) &x;
    if (*c == 0x78) {
        printf("little endian");
    } else {
        printf("big endian");
    }
    return 0;
}