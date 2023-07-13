cmake_minimum_required(VERSION 3.23)
project(helpy)

set(CMAKE_CXX_STANDARD 17)

add_executable(helpy
        src/main.cpp
        src/manager/manager.cpp
        src/manager/manager.h
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/lexer/token.hpp)
