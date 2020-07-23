cmake_minimum_required(VERSION 3.10)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there. You may need to remove CMakeCache.txt.")
endif ()

project(cryptopals)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

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

find_package(Catch2 CONFIG REQUIRED)
#find_package(fmt CONFIG REQUIRED)

add_library(utils STATIC utils/utils.cpp utils/utils.h)
target_include_directories(utils INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>)

set(DEFAULT_LIBS
        CONAN_PKG::range-v3
        Catch2::Catch2
        CONAN_PKG::fmt
        utils)

add_executable(1.hex_to_base64 1.basics/1.hex_to_base64.cpp)
target_link_libraries(1.hex_to_base64 PRIVATE ${DEFAULT_LIBS})

add_executable(2.fixed_xor 1.basics/2.fixed_xor.cpp)
target_link_libraries(2.fixed_xor PRIVATE ${DEFAULT_LIBS})

add_executable(3.single-byte_xor_cipher 1.basics/3.single-byte_xor_cipher.cpp 1.basics/byte_xor.cpp)
target_link_libraries(3.single-byte_xor_cipher PRIVATE ${DEFAULT_LIBS})

add_executable(4.single-byte_xor_detect 1.basics/4.single-byte_xor_detect.cpp 1.basics/byte_xor.cpp)
target_link_libraries(4.single-byte_xor_detect PRIVATE ${DEFAULT_LIBS})
file(COPY 1.basics/4.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(5.repeating-key_xor 1.basics/5.repeating-key_xor.cpp)
target_link_libraries(5.repeating-key_xor PRIVATE ${DEFAULT_LIBS})

add_executable(6.repeating-key_xor_break 1.basics/6.repeating-key_xor_break.cpp)
target_link_libraries(6.repeating-key_xor_break PRIVATE ${DEFAULT_LIBS})
file(COPY 1.basics/6.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (clang_tidy)
    set_target_properties(
            utils
            1.hex_to_base64
            2.fixed_xor
            3.single-byte_xor_cipher
            PROPERTIES CXX_CLANG_TIDY ${clang_tidy}
    )
endif (clang_tidy)
