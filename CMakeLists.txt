cmake_minimum_required(VERSION 3.16.3)
project(helpy)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_DEPENDENCIES
        external/libfort/fort.hpp
        external/libfort/fort.c)

set(PROJECT_HEADERS
        src/lexer/lexer.h
        src/manager/manager.h
        src/parser/parser.h
        src/utils/command.hpp
        src/utils/token.hpp
        src/utils/utils.hpp
        src/writer/writer.h)

set(PROJECT_SOURCES
        src/lexer/lexer.cpp
        src/main.cpp
        src/manager/manager.cpp
        src/parser/parser.cpp
        src/writer/writer.cpp)

add_executable(helpy
        ${PROJECT_DEPENDENCIES}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES})

# for testing purposes
add_executable(test
        cli/main.cpp
        cli/my_helpy.cpp
        cli/my_helpy.h
        cli/my_helpy_utils.hpp
        external/libfort/fort.c
        external/libfort/fort.hpp)
