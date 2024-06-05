cmake_minimum_required(VERSION 3.22)

project(VHelp)

file(GLOB_RECURSE sources source/*)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(vhelp ${sources})

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(vhelp PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
