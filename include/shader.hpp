#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

GLuint loadShader(GLenum type, const std::string& path);
GLuint loadProgram(std::vector<GLuint> shaders);
