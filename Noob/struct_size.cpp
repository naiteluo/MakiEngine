#include <iostream>

// preprocessor to modify struct bytes alignment rule
// align by 1 bytes
#pragma pack(push, 1)
typedef struct _A {
    uint16_t a; // 2
    uint32_t b; // 4
    uint32_t c; // 4
    uint32_t d; // 4
} A; // total bytes: 14

typedef struct _B_MODIFIED {
    uint8_t a1;  // 1
    uint16_t b1; // 2
    uint16_t b2; // 2
    uint32_t c1; // 4
    uint32_t c2; // 4
    uint32_t d3; // 4
} B_MODIFIED; // total bytes: 17

#pragma pack(pop)
// end of preprocessor

// default bytes alignment
typedef struct _B_DEFAULT {
    uint8_t a1;  // 1
    uint16_t b1; // 2
    uint16_t b2; // 2
    uint32_t c1; // 4
    uint32_t c2; // 4
    uint32_t d3; // 4
} B_DEFAULT; // total bytes: 20

int main() {
    std::cout << "sizeof(A): " << sizeof(A) << std::endl << std::endl;

    std::cout << "sizeof(B_DEFAULT): " << sizeof(B_DEFAULT) << std::endl << std::endl;

    B_DEFAULT b_default{
            0x11,
            0x2233,
            0x4455,
            0x66778899,
            0xAABBCCDD,
            0xEEFF0011
    };
    // Memory View:
    //                                11 00 33 22   55 44 00 00
    //    99 88 77 66   dd cc bb aa   11 00 ff ee

    size_t n1 = sizeof(B_DEFAULT);
    for (size_t i = 0; i < n1; ++i) {
        uint8_t *b1 = (uint8_t *) (&b_default) + i;
        if (i % 4 == 0) {
            printf("%016x:   ", b1);
        }
        printf("%02x ", *b1);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }

    std::cout << "\n---------------------------------\n\n";

    std::cout << "sizeof(B_MODIFIED): " << sizeof(B_MODIFIED) << std::endl << std::endl;
    B_MODIFIED b_modified{
            0x11,
            0x2233,
            0x4455,
            0x66778899,
            0xAABBCCDD,
            0xEEFF0011
    };
    // Memory View:
    //    11 33 22 55   44 99 88 77   66 dd cc bb   aa 11 00 ff
    //    ee

    size_t n2 = sizeof(B_MODIFIED);
    for (size_t i = 0; i < n2; ++i) {
        uint8_t *b1 = (uint8_t *) (&b_modified) + i;
        if (i % 4 == 0) {
            printf("%016x:   ", b1);
        }
        printf("%02x ", *b1);
        if ((i + 1) % 4 == 0) {
            printf("\n");
        }
    }

    std::cout << std::endl;
    return 0;
}
