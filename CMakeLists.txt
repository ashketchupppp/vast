cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMKAE_MAKE_PROGRAM Ninja)
set(CMAKE_PREFIX_PATH "./vcpkg_installed/x64-windows")

project(Vast)

find_package(OpenCV CONFIG REQUIRED)

add_subdirectory(src)
