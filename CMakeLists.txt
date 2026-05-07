cmake_minimum_required(VERSION 3.20)
project(Space-Strategy-Game)

set(CMAKE_CXX_STANDARD 23)

file(GLOB src_files "src/*.cpp")

add_executable(Space_Game ${src_files})