cmake_minimum_required(VERSION 3.10)
project(cryptopals)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Catch2 CONFIG REQUIRED)

add_executable(1.hex_to_base64 1.basics/1.hex_to_base64.cpp)
target_link_libraries(1.hex_to_base64 PRIVATE Catch2::Catch2)
