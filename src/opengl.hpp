#include <iostream>
#include <chrono>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "load_stb_image.hpp"
#include "shader.hpp"
#include "videoio.hpp"

unsigned int createShader(int shaderType) {
    unsigned int shader;
    shader = glCreateShader(shaderType);
    return shader;
}

int createShaderProgram(
    const char* vertexShaderSource,
    const int vertexShader,
    const char* fragmentShaderSource,
    const int fragmentShader
) {
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create a shader program object, which will link our two shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    return shaderProgram;
}

int opengl (std::string inputFile, std::string outputFile) {
    // Now do OpenGL stuff
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    int wWidth = 800;
    int wHeight = 600;

    GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "LearnOpenGL", NULL, NULL);
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

    // --- Compile our shaders as early as possible ---
    Shader shader;
    try {
        shader = Shader("./shader.vs", "./shader.fs");
    } catch (...) {
        Log::error("Failed to compile shaders.");
        return 1;
    }

    glViewport(0, 0, wWidth, wHeight);

    // --- Create our triangle buffer VBO and VAO ---
    float vertices[] = {
        // positions          // colors           // texture coords
        1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --- Load our single rectangle texture ---
    unsigned int texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Video inputVid(inputFile);
    const int readRc = inputVid.readOpen();
    if (!readRc) {
        return 1;
    }
    double targetFps = inputVid.fps();
    double t0 = glfwGetTime();
    std::cout << "TARGET FPS: " << inputVid.fps() << std::endl;

    // Read the first frame
    cv::Mat frame = inputVid.readFrame();
    while(!glfwWindowShouldClose(window)) {
        std::chrono::milliseconds startTime = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );

        // --- Generate OpenGL texture from cv::Mat ---
        //use fast 4-byte alignment (default anyway) if possible
        glPixelStorei(GL_UNPACK_ALIGNMENT, (frame.step & 3) ? 1 : 4);

        //set length of one complete row in data (doesn't need to equal image.cols)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, frame.step/frame.elemSize());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // --- Clear viewport ---
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // --- Use our texture and Shaders --- 
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();

        // --- Update uniforms for the shader ---
        float timeValue = glfwGetTime();
        int vecLocation = glGetUniformLocation(shader.id(), "time");
        glUniform1fv(vecLocation, sizeof timeValue, &timeValue);

        // --- Draw stuff
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();

        Log::perf(" render: " + std::to_string(glfwGetTime() - t0));

        // --- Limit FPS to input video FPS ---
        bool frameLoaded = false;
        while (glfwGetTime() < t0 + (1.0/targetFps)) {
            if (!frameLoaded) {
                frame = inputVid.readFrame();
                frameLoaded = true;
            }
        }

        Log::perf(" total: " + std::to_string(glfwGetTime() - t0), true);

        t0 = glfwGetTime();
    }

    glfwTerminate();
    return 0;
}