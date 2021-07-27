#include <stdio.h>

union u {
    int a;  // 4
    char b; // 1
};

int main() {
    // method 1
    unsigned int x = 0x12345678;
    char *c = (char *) &x;
    if (*c == 0x78) {
        printf("little endian");
    } else {
        printf("big endian");
    }

    // method 2
    u u1;
    u1.a = 1;
    if (u1.b == 1) {
        printf("little endian");
    } else {
        printf("big endian");
    }
    return 0;
}