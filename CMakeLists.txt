######################################################################
# @author      : MasrulHuda (MasrulHuda@iMacHome)
# @file        : CMakeLists
# @created     : Wednesday Mar 29, 2023 03:10:13 CDT
######################################################################


cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)


project("inception")
set(CMAKE_VERBOSE_MAKEFILE ON)

# Optimization flag 
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-O2 -Wall  -std=c++20")
    set(CMAKE_C_FLAGS "-g -O2")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-g  -O3 -std=c++20")
    set(CMAKE_C_FLAGS "-g -O2")
endif()

file(GLOB SRC "src/*.cpp")
include_directories("./src/")
add_executable(${PROJECT_NAME}  ${SRC})

