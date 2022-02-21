cmake_minimum_required(VERSION 3.21)
project(CircularQueue)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # clang-cl
        message(STATUS "clang-cl frontend detected")
        set(COMPILER_FLAGS "/EHs /O2 -Wno-unknown-argument")
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") # clang native
        message(STATUS "clang with GNU frontend detected")
        set(COMPILER_FLAGS "-Wall -Wpedantic -O2")
    endif()
else()
    set(COMPILER_FLAGS "-Wall -Wpedantic -O2")
endif()


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if (ENABLE_UNIT_TESTS)
    enable_testing()
    file(GLOB_RECURSE TESTS test/*.cpp)
    add_subdirectory(test)
endif ()

file(GLOB_RECURSE SOURCES
        src/*.hpp
        src/*.cpp)
list(REMOVE_ITEM SOURCES "src/main.cpp")

add_executable(CircularQueue ${SOURCES} src/main.cpp)
target_include_directories(CircularQueue PRIVATE include)
