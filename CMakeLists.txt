######################################################################
# @author      : MasrulHuda (MasrulHuda@iMacHome)
# @file        : CMakeLists
# @created     : Wednesday Mar 29, 2023 03:10:13 CDT
######################################################################


cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project("inception")
set(CMAKE_VERBOSE_MAKEFILE ON)

# Optimization flag 
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-O2 -Wall  -std=c++17")
    set(CMAKE_C_FLAGS "-g -O2")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-g  -O2 -std=c++17")
    set(CMAKE_C_FLAGS "-g -O2")
endif()

file(GLOB SRC "src/*.cpp")
include_directories("./src/")
add_executable(${PROJECT_NAME}  ${SRC})

