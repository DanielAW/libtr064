cmake_minimum_required (VERSION 3.0)
#project(libtr064)

find_package(CURL REQUIRED)
#message("CURL Found? " ${CURL_FOUND})
find_package(LibXml2 REQUIRED)
#message("LibXml2 Found? " ${LIBXML2_FOUND})

# in Arch Linux e.g. xmlreader.h is located in /usr/include/libxml2/libxml
# NOT in /usr/include/libxml
include_directories(${LIBXML2_INCLUDE_DIR})

add_library(tr064 SHARED connect.c xml-parser.c)
target_link_libraries(tr064 curl xml2)
