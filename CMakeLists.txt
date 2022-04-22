cmake_minimum_required(VERSION 3.22)
project(os_wet1)

set(CMAKE_CXX_STANDARD 11)

add_executable(os_wet1 smash commands.h signals.h smash.cpp signals.cpp commands.cpp)
