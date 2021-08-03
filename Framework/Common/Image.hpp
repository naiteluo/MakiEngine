#pragma once

#include "GeomMath.h"
#include <iostream>

namespace Me {
    typedef struct _Image {
        uint32_t Width;
        uint32_t Height;
        R8G8B8A8Unorm *data;
        uint32_t bit_count;
        uint32_t pitch;
        size_t data_size;
    } Image;

    std::ostream &operator<<(std::ostream &out, Image img);
}