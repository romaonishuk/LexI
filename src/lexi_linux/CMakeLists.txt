project(lexi_linux)

# interface lib
add_library(${PROJECT_NAME}_interface INTERFACE)
target_include_directories(lexi_interface INTERFACE inc)

add_library(${PROJECT_NAME} STATIC
        src/x_window_impl.cpp
        src/event_manager.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lexi_interface X11)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES} ${PROJECT_NAME}_interface)