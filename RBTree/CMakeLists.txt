cmake_minimum_required(VERSION 3.27)
project(RBTree)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(SFML 2.6 REQUIRED graphics window system)
find_package(Threads REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

add_executable(RBTree
        Main.cpp
        IODialog.h
        RBNode.h)

target_link_libraries(RBTree sfml-graphics sfml-window sfml-system Threads::Threads)
