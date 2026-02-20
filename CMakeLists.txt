cmake_minimum_required(VERSION 3.14)
project(CppMulti VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put all built binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ── Optional AddressSanitizer ─────────────────────────────────────────────────
# Usage:  cmake ... -DENABLE_ASAN=ON
#
# Uses -static-libasan so the build works even when libasan.so is not installed
# (common on a minimal RHEL 8 / CentOS 8 installation).  If you have
# libasan installed (dnf install libasan) you can replace -static-libasan
# with just -fsanitize=address.
option(ENABLE_ASAN "Build with AddressSanitizer" OFF)
if(ENABLE_ASAN)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "ENABLE_ASAN is most useful with -DCMAKE_BUILD_TYPE=Debug")
    endif()
    message(STATUS "AddressSanitizer enabled (static-libasan)")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -static-libasan)
    add_link_options   (-fsanitize=address -static-libasan)
endif()

add_subdirectory(geometry)
add_subdirectory(io)
add_subdirectory(app)
