#pragma once

#include "Interface.hpp"
#include "Image.hpp"
#include "Buffer.hpp"

namespace Me {
    Interface ImageParser {
    public:
        virtual Image Parse(const Buffer &buf) = 0;
    };
}