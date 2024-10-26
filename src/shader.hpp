#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "logging.hpp"

class Shader {
public:
    Shader() {}

    Shader (const char * vertexPath, const char * fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();	            
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }        
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(fragment, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        if (checkCompileErrors(fragment, "FRAGMENT")) throw;

        shaderId = glCreateProgram();
        glAttachShader(shaderId, vertex);
        glAttachShader(shaderId, fragment);

        glLinkProgram(shaderId);
        if (checkCompileErrors(fragment, "PROGRAM")) throw;

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    unsigned int id() { return shaderId; }

    void use() { 
        glUseProgram(shaderId); 
    }

private:
    int checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        char infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                Log::error("SHADER_COMPILATION_ERROR of type: " + type + "\n" + std::string(infoLog));
                return 1;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Log::error("PROGRAM_LINKING_ERROR of type: " + type + "\n" + std::string(infoLog));
                return 1;
            }
        }
        return 0;
    }

    unsigned int shaderId;
};