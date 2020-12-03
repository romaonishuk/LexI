project(lexi_windows)

add_library(${PROJECT_NAME} STATIC src/w_window_impl.cpp src/event_manager.cpp src/menu_item.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC lexi_interface)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
