cmake_minimum_required(VERSION 3.20.0)
project(openfiles VERSION 5.0.1 DESCRIPTION "Multi-Platform Event-Driven Application Framework")

if (NOT DEFINED OPENFILE_CONFIG)
   set(OPENFILE_CONFIG "configs/default")
endif()

include(${OPENFILE_CONFIG})
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
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(of_core_linux)
    add_subdirectory(of_core_fs_linux)
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_subdirectory(of_core_windows)
    add_subdirectory(of_core_fs_windows)
endif ()
add_subdirectory(of_core_fs_bookmarks)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "Open Files")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_PROJECT_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/documentation/connected.png)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_FULL_SIDEBAR YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/documentation/headerFile)
    set(DOXYGEN_INTERNAL_DOCS NO)
    list(APPEND DOXYGEN_HTML_EXTRA_FILES ${CMAKE_CURRENT_SOURCE_DIR}/documentation/connected.ico)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.openfiles)
    set(DOXYGEN_EXAMPLE_PATH ${of_core_SOURCE_DIR}/test)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation)
    set(DOXYGEN_EXCLUDE_PATTERNS
	*/file_call.h
	*/net_internal.h
	*/fs.h
	*/fstypes.h
	*/impl/*
	*.c
	README.md
	)

    doxygen_add_docs(
        openfiles
	${CMAKE_CURRENT_SOURCE_DIR}/documentation/openfiles.h
	${CMAKE_CURRENT_SOURCE_DIR}/documentation/config.h
        of_core
	)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_target(
            doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
            )
    endif()
endif()

