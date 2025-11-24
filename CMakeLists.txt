cmake_minimum_required(VERSION 3.26)
project(KiloR)

set(CMAKE_CXX_STANDARD 14)

add_executable(KiloR
        src/Editor.cpp
        src/main.cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(KiloR ${CURSES_LIBRARIES})
