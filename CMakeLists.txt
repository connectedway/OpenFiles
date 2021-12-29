cmake_minimum_required(VERSION 3.21.1)
project(openfiles VERSION 1.0.1 DESCRIPTION "OpenFiles Stack")

include(configs/default)
configure_file(include/ofc/config.h.in ofc/config.h @ONLY)
include_directories(${openfiles_BINARY_DIR})

add_subdirectory(of_core)
include_directories(${of_core_SOURCE_DIR}/include)

add_subdirectory(of_core_binheap)
add_subdirectory(of_core_cheap)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_subdirectory(of_core_darwin)
    add_subdirectory(of_core_fs_darwin)
endif ()
add_subdirectory(of_core_fs_bookmarks)
