cmake_minimum_required(VERSION 4.0)
project(SystemsProg)

set(CMAKE_CXX_STANDARD 20)

add_executable(SystemsProg Prog1/main.cpp
        Prog2/Params.hpp
        Prog2/Params.cpp
        Prog2/main.cpp
        Prog3/Direntry.cpp)

add_executable(sweeper
        Prog3/Stats.h
        Prog3/Stats.cpp
        Prog3/Direntry.h
        Prog3/Direntry.cpp
        Prog3/FileID.h
        Prog3/FileID.cpp
        Prog3/Sweeper.h
        Prog3/Sweeper.cpp
        Prog3/Params.cpp
        Prog3/Params.hpp
        Prog3/main.cpp)
add_executable(Musical
        Prog5/Model.h
        Prog5/Kid.h
        Prog5/Kid.cpp
        Prog5/Mom.cpp)
