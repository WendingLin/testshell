cmake_minimum_required(VERSION 3.12)
project(testshell)

set(CMAKE_CXX_STANDARD 98 )
set(-W -Wall -Wextra -pedantic)

add_executable(testshell main.cpp)