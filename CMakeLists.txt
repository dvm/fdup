cmake_minimum_required(VERSION 3.20)

project(fdup LANGUAGES CXX VERSION 1.5.0)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Set build type to Release or Debug" FORCE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    add_compile_options(-Wall -Wextra -pedantic)
endif(UNIX)

set(SOURCES
    "fdup.cpp"
    "stopwatch.h"
    "filehasher.h"
    "gatherer.h"
    "gatherer.cpp"
    "finder.h"
    "finder.cpp"
    "viewer.h"
    "viewer.cpp"
    "pipe.h"
    "flags.h"
    "options.h"
    "options.cpp"
)

add_executable(fdup ${SOURCES})
