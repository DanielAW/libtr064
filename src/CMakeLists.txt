cmake_minimum_required (VERSION 3.0)
project(tr064)

#set(CMAKE_C_FLAGS "-std=c11 -Wall ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "-std=gnu11 -Wall ${CMAKE_C_FLAGS}")

add_subdirectory(lib)
add_subdirectory(cli)
