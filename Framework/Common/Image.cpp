#include "Image.hpp"

std::ostream &Me::operator<<(std::ostream &out, Me::Image img) {
    out << "Image:\n";
    out << "\tWidth: " << img.Width << '\n';
    out << "\tHeight: " << img.Height << '\n';
    out << "\tdata size: " << sizeof(img.data) << '\n';
    out << "\tbit_count: " << img.bit_count << '\n';
    out << "\tpitch: " << img.pitch << '\n';
    out << "\tdata_zise: " << img.data_size << '\n';
    return out;
}