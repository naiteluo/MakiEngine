#pragma once

#include "Interface.hpp"

namespace Me {
    Interface IRuntimeModule {
    public:
        virtual ~IRuntimeModule() = default;;

        virtual int Initialize() = 0;

        virtual void Finalize() = 0;

        virtual void Tick() = 0;
    };
}