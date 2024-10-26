add_executable(Vast main.cpp)

target_link_libraries(Vast PRIVATE
    opencv_videoio
    opencv_ml
    opencv_dnn
    opencv_core
    glfw
    glad::glad
)

configure_file(resources/shader.fs shader.fs COPYONLY)
configure_file(resources/shader.vs shader.vs COPYONLY)