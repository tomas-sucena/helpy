cmake_minimum_required(VERSION 3.16.3)
project(helpy)

set(CMAKE_CXX_STANDARD 17)

add_executable(helpy
        src/main.cpp
        src/manager/manager.cpp
        src/manager/manager.h
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/utils/token.hpp
        src/parser/parser.cpp
        src/parser/parser.h
        src/utils/command.hpp
        src/writer/writer.cpp
        src/writer/writer.h
        src/utils/utils.hpp)
