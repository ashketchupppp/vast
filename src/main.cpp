#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "load_stb_image.hpp"
#include "framedifference.hpp"
#include "logging.hpp"

struct Options {
    bool validArguments;
    std::string inputFile;
    std::string outputFile;
};

Options parseArgs (int argc, char** argv) {
    // ignore first argument, that's the exe path
    argv++;

    Options opts;
    opts.validArguments = true;

    // Put argv into args vector
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(*argv);
        argv++;
    }

    if (args.size() < 2) {
        Log::error("Not enough arguments!");
        Log::error("Usage: Vast <input-file> <output-file>");
        opts.validArguments = false;
        return opts;
    }

    opts.inputFile = args[0];
    opts.outputFile = args[1];

    return opts;
}

bool validateOptions(Options opts) {
    if (!std::filesystem::exists(opts.inputFile)) {
        Log::error(opts.inputFile + " does not exist");
        return false;
    }
    // Do this check when command line parsing is better and its easier to add a --force option
    // if (std::filesystem::exists(opts.outputFile)) {
    //     Log::error(opts.outputFile + " already exists");
    //     return false;
    // }
    return true;
}

int opengl () {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // Create our triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create our vertex shader
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Create our fragment shader
    const char * fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create a shader program object, which will link our two shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Use Vertex Buffer Object if you so wish...
    // copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // then set the vertex attributes pointers - this is telling OpenGL how to pass things to our shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    // use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    */
    // Use Vertex Buffer Array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();    
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

    glfwTerminate();
    return 0;
}

int main(int argc, char** argv)
{
    return opengl();

    Options opts = parseArgs(argc, argv);

    // What are you asking for?
    if (!opts.validArguments) {
        return 1;
    }

    // Is what you are asking for valid?
    if (!validateOptions(opts)) {
        return 1;
    }

    // Do frame difference

    return frameDifference(opts.inputFile, opts.outputFile);
}