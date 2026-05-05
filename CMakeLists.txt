cmake_minimum_required(VERSION 3.20)
project(Space-Strategy-Game)

set(CMAKE_CXX_STANDARD 23)


include_directories(include)
include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Space_Game ${SOURCES})
