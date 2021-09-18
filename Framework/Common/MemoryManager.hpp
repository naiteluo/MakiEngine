#pragma once

#include "IRuntimeModule.hpp"
#include "Allocator.hpp"
#include <new>

namespace Me {
    class MemoryManager : implements IRuntimeModule {
    public:

        template<typename T, typename ... Arguments>
        T *New(Arguments... parameters) {
            return new(Allocate(sizeof(T))) T(parameters...);
        }

        template<typename T>
        void Delete(T *p) {
            reinterpret_cast<T *>(p)->~T();
            Free(p, sizeof(T));
        }

    public:
        virtual ~MemoryManager() {}

        virtual int Initialize();

        virtual void Finalize();

        virtual void Tick();

        void *Allocate(size_t size);

        void *Allocate(size_t size, size_t alignment);

        void Free(void *p, size_t size);

    private:
        static size_t *m_pBlockSizeLoopUp;
        static Allocator *m_pAllocators;
    private:
        static Allocator *LoopUpAllocator(size_t size);
    };

    extern MemoryManager *g_pMemoryManager;
}