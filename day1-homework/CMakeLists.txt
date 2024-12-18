cmake_minimum_required (VERSION 3.12)

project (highlevel)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_VERBOSE_MAKEFILE on )

# Initialize the CMake library for the KasperskyOS SDK.
include (platform)

# Statically link executable files.
initialize_platform (FORCE_STATIC)

# Connect the CMake library named nk for working with the NK compiler.
include (platform/nk)

project_header_default ("STANDARD_GNU_17:YES" "EXTRA_STRICT_WARNINGS:YES")

set (RESOURCES ${CMAKE_SOURCE_DIR}/resources)

# During the build, the generated code of components will be put in the module that is indicated using the NK_MODULE parameter.
# For this reason, the names of components must have a prefix matching the specified module
# so that they can be found by the build system.

# Include the directory with the generated config header files.
include_directories (${CMAKE_CURRENT_BINARY_DIR})

# Include the example directory.
include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (kettle)
add_subdirectory (keys)
add_subdirectory (checker)
add_subdirectory (network)
add_subdirectory (einit)
