cmake_minimum_required(VERSION 3.6)
project(mod1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SDL2_INCLUDE_DIR ~/.brew/include/SDL2)
#set(SDL2_LIBRARY ~/.brew/lib/libSDL2.so)
# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ~/goinfre/Softs/collector-master/cmake)

set(SOURCE_FILES src/main.cpp src/map.cpp includes/map.h includes/mod1.h)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(mod1 ${SOURCE_FILES})
target_link_libraries(SDL2_App ${SDL2_LIBRARY})