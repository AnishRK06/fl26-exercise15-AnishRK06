cmake_minimum_required(VERSION 3.5)
project(Exercise15 CXX)

# require a C++14 compiler for all targets
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(test_vector test_vector.cpp thread_safe_vector.hpp)

target_link_libraries (test_vector ${CMAKE_THREAD_LIBS_INIT})