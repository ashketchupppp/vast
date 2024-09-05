cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(opencv CONFIG REQUIRED)

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)