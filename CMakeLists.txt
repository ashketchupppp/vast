cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Vast)

find_package(opencv CONFIG REQUIRED)

add_subdirectory(src)