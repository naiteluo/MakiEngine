add_definitions(-D_DEBUG)

add_library(Common
        Allocator.cpp
        AssetLoader.cpp
        BaseApplication.cpp
        GraphicsManager.cpp
        Image.cpp
        MemoryManager.cpp
        Scene.cpp
        SceneManager.cpp
        SceneObject.cpp
        main.cpp
        )

target_link_libraries(Common GeomMath)
me_add_xg_platform_dependencies(Common)

add_subdirectory(test)