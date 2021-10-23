cmake_minimum_required(VERSION 3.10)
project(Stack)

set(CMAKE_CXX_STANDARD 11)

add_executable(Stack
        src/StackFunc.cpp
        src/StackMain.cpp
        src/StackTest.cpp
        include/Stack.h)