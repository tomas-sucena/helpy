cmake_minimum_required(VERSION 3.23)
project(helpy)

set(CMAKE_CXX_STANDARD 17)

add_executable(helpy main.cpp src/manager.cpp src/manager.h)
