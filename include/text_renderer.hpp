#pragma once

#include "shader.hpp"
#include "font.hpp"

class TextRenderer
{
public:
    TextRenderer(int screen_width, int screen_height);
    ~TextRenderer();

    void draw(const std::string& message, int x, int y, Font* font);

private:
    int screen_width_ = 0;
    int screen_height_ = 0;

    GLuint program_ = 0u;
    GLuint vao_ = 0u;
    GLuint vbo_ = 0u;

    GLint screen_width_location_ = -1;
    GLint screen_height_location_ = -1;
    GLint uv_rects_location_ = -1;
    GLint screen_rects_location_ = -1;
    GLint atlas_location_ = -1;
};
