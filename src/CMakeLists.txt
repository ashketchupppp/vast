add_executable(Vast main.cpp)

target_link_libraries(Vast PRIVATE opencv_videoio opencv_ml opencv_dnn opencv_core)