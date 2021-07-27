#pragma once
#include "GeomMath.h"
#include <iostream>

namespace Me {
    typedef  struct _Image {
        uint32_t Width;
        uint32_t Height;
        R8G8B8A8Unorm* data;
        uint32_t bit_count;
        uint32_t pitch;
        size_t data_size;
    } Image;

    std::ostream & operator << (std::ostream& out, Image img) {
        out << "Image:\n";
        out << "\tWidth: " << img.Width << '\n';
        out << "\tHeight: " << img.Height << '\n';
        out << "\tdata size: " << sizeof(img.data) << '\n';
        out << "\tbit_count: " << img.bit_count << '\n';
        out << "\tpitch: " << img.pitch << '\n';
        out << "\tdata_zise: " << img.data_size << '\n';
        return out;
    }
}