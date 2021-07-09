include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(hello_xcb hello_xcb.cpp)
target_link_libraries(hello_xcb xcb X11 X11-xcb dl)