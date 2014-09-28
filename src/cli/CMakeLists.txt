cmake_minimum_required (VERSION 3.0)
#project(tr064cli)

add_executable(tr064cli cli.cpp)
target_link_libraries(tr064cli tr064)
