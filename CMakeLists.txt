cmake_minimum_required(VERSION 3.10)
project(LexI)

set(CMAKE_CXX_STANDARD 17)

# TODO(rmn): sanitizers
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O3")
else()
    set(CMAKE_CXX_FLAGS "-O0")
endif()
# TODO(rmn): remove Wno
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-narrowing -Wno-sign-compare -Wno-unused-variable")
message(${CMAKE_CXX_FLAGS})

# interface lib
add_library(lexi_interface INTERFACE)
target_include_directories(lexi_interface INTERFACE inc inc/glyphs)

set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src)

FILE(GLOB LEXI_SOURCES ${PROJECT_SOURCES}/*.cpp)
add_executable(${PROJECT_NAME} main.cpp ${LEXI_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE lexi_interface)

# Platform dependent stuff
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(src/lexi_linux)

    target_link_libraries(${PROJECT_NAME} PRIVATE lexi_linux)
else ()
    message(FATAL "Unsupported System")
endif ()

# TODO(rmn): add installation rules
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()
