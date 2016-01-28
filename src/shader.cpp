#include <iostream>
#include <fstream>
#include "error.hpp"
#include "shader.hpp"

GLuint loadShader(GLenum type, const std::string& path)
{
    std::vector<GLchar> buffer;

    std::ifstream file (path);
    if (!file.is_open())
    {
        fatalError("Failed to open shader '" + path +"'.");
    }
    GLint file_length = 0;
    file.seekg(0, std::ios::end);
    file_length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer.resize(file_length);
    file.read(buffer.data(), file_length);

    const GLchar* source = buffer.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &file_length);
    glCompileShader(shader);
    
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        buffer.resize(log_length);
        glGetShaderInfoLog(shader, log_length, nullptr, buffer.data());
        std::string info_log (buffer.data(), log_length);
        std::cerr << info_log;
        fatalError("Failed to compile shader '" + path +"'.");
    }
    return shader;
}

GLuint loadProgram(std::vector<GLuint> shaders)
{
    GLuint program = glCreateProgram();;
    for (GLuint shader : shaders)
    {
        glAttachShader(program, shader); 
    }
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        int log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> buffer (log_length);
        glGetProgramInfoLog(program, log_length, nullptr, buffer.data());
        std::string info_log (buffer.data(), log_length);

        fatalError("Failed to link program.");
    }

    for (GLuint shader : shaders)
    {
        glDetachShader(program, shader); 
    }
    return program;
}
