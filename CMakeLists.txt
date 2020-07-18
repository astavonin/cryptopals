cmake_minimum_required(VERSION 3.10)
project(cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Catch2 CONFIG REQUIRED)

find_program(clang_tidy NAMES clang-tidy PATHS /usr/local/opt/llvm/bin/)

message("\n#####################################################\n")
message("CMAKE_BUILD_TYPE:\t${CMAKE_BUILD_TYPE}")
message("CMAKE_INSTALL_PREFIX:\t${CMAKE_INSTALL_PREFIX}")
message("CMAKE_BINARY_DIR:\t${CMAKE_BINARY_DIR}")
message("CMAKE_MODULE_PATH:\t${CMAKE_MODULE_PATH}")
message("CMAKE_PREFIX_PATH:\t${CMAKE_PREFIX_PATH}")
message("CMAKE_SYSTEM_NAME:\t${CMAKE_SYSTEM_NAME}")
message("CMAKE_GENERATOR:\t${CMAKE_GENERATOR}")
message("CMAKE_C_COMPILER:\t${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:\t${CMAKE_CXX_COMPILER}")
message("CLANG-TIDY:\t\t${clang_tidy}")
message("\n#####################################################\n")


add_executable(1.hex_to_base64 1.basics/1.hex_to_base64.cpp 1.basics/utils.cpp)
target_link_libraries(1.hex_to_base64 PRIVATE Catch2::Catch2)

add_executable(2.fixed_xor 1.basics/2.fixed_xor.cpp 1.basics/utils.cpp)
target_link_libraries(2.fixed_xor PRIVATE Catch2::Catch2)

if (clang_tidy)
    set_target_properties(
            1.hex_to_base64
            2.fixed_xor
            PROPERTIES CXX_CLANG_TIDY ${clang_tidy}
    )
endif (clang_tidy)
