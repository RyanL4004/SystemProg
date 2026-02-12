cmake_minimum_required(VERSION 4.0)
project(SystemsProg)

set(CMAKE_CXX_STANDARD 20)

add_executable(SystemsProg Prog1/main.cpp
        Prog2/Params.hpp
        Prog2/Params.cpp
        Prog2/main.cpp)
