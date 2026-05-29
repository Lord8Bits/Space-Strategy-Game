cmake_minimum_required(VERSION 3.20)

project(Space_Strategy_Game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Space_Game ${SOURCES})

target_compile_options(Space_Game PRIVATE -Wall -O2)
target_include_directories(Space_Game PRIVATE include)