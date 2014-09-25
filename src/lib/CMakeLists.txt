cmake_minimum_required (VERSION 3.0)
project(libtr064)

include(FindCURL)

find_package(CURL REQUIRED)
if(NOT CURL_FOUND)
    message(FATAL_ERROR "Curl not found!")
endif(NOT CURL_FOUND)

add_library(libtr064 SHARED connect.c)
target_link_libraries(libtr064 curl)
