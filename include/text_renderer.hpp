#pragma once

#include <glm/glm.hpp>
#include "font.hpp"
#include "shader.hpp"

class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();

    void draw(
            const glm::mat4& viewport,
            const std::string& message, 
            const glm::vec2& position, 
            const glm::vec4& color,
            const Font* font);

private:
    GLuint program_ = 0u;
    GLuint vao_ = 0u;
    GLuint vbo_ = 0u;

    GLint viewport_location_ = -1;
    GLint color_location = -1;
    GLint atlas_bounds_location_ = -1;
    GLint screen_bounds_location_ = -1;
    GLint atlas_location_ = -1;
};
