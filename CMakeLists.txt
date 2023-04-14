cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)


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
install(TARGETS inception DESTINATION ${CMAKE_INSTALL_PREFIX})

