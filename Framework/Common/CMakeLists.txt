add_definitions(-D_DEBUG)

add_library(Common
        Allocator.cpp
        BaseApplication.cpp
        GraphicsManager.cpp
        MemoryManager.cpp
        AssetLoader.cpp
        main.cpp
        )

target_link_libraries(Common GeomMath)



add_subdirectory(test)