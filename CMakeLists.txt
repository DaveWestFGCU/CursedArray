cmake_minimum_required(VERSION 3.27)
project(CursedArray)

set(CMAKE_CXX_STANDARD 17)

add_executable(CursedArray testbed_main.cpp
        CursedArray.cpp
        List.h
        Queue.h
        RedBlack_Tree.h
        )
