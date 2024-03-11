cmake_minimum_required(VERSION 3.26)
project(lambda_NFA)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(lambda_NFA
        include/lnfa.h
        include/state.h
        src/lnfa.cpp
        src/main.cpp
        src/state.cpp
        include/state.h)
