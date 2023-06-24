cmake_minimum_required(VERSION 3.5)

project(fdup LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Set build type to Release or Debug" FORCE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED Crypto)

set(SOURCES fdup.cpp)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(fdup ${SOURCES})
target_link_libraries(fdup OpenSSL::Crypto)
