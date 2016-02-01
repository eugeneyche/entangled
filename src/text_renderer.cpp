#include <vector>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "text_renderer.hpp"

TextRenderer::TextRenderer()
{
    GLuint vert_shader = loadShader(GL_VERTEX_SHADER, "res/shader/text.vert");
    GLuint frag_shader = loadShader(GL_FRAGMENT_SHADER, "res/shader/text.frag");
    program_ = loadProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    viewport_location_ = glGetUniformLocation(program_, "viewport");
    color_location = glGetUniformLocation(program_, "color");
    atlas_bounds_location_ = glGetUniformLocation(program_, "atlas_bounds");
    screen_bounds_location_ = glGetUniformLocation(program_, "screen_bounds");
    atlas_location_ = glGetUniformLocation(program_, "atlas");

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    std::vector<GLfloat> vertex_buffer = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0
    };
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * vertex_buffer.size(),
            vertex_buffer.data(),
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0u);
}

TextRenderer::~TextRenderer()
{
    glDeleteProgram(program_);
}

void TextRenderer::draw(
        const glm::mat4& viewport, 
        const std::string& message, 
        const glm::vec2& position, 
        const glm::vec4& color, 
        const Font* font)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<GLfloat> atlas_bounds;
    std::vector<GLfloat> screen_bounds;
    float cursor_x = position[0];
    float cursor_y = position[1];
    for (size_t i = 0; i < message.size(); i++)
    {
        GlyphMetrics metrics = font->getGlyphMetrics(message[i]);
        GlyphBound bound = font->getGlyphBound(message[i]);

        atlas_bounds.push_back(bound.min_s);
        atlas_bounds.push_back(bound.max_s);
        atlas_bounds.push_back(bound.min_t);
        atlas_bounds.push_back(bound.max_t);

        screen_bounds.push_back(cursor_x + metrics.min_x);
        screen_bounds.push_back(cursor_x + metrics.max_x);
        screen_bounds.push_back(cursor_y + metrics.min_y);
        screen_bounds.push_back(cursor_y + metrics.max_y);

        cursor_x += metrics.advance;
    }
    glUseProgram(program_);
    glUniformMatrix4fv(viewport_location_, 1, GL_FALSE, glm::value_ptr(viewport));
    glUniform3fv(color_location, 1, glm::value_ptr(color));
    glUniform4fv(atlas_bounds_location_, message.size(), atlas_bounds.data());
    glUniform4fv(screen_bounds_location_, message.size(), screen_bounds.data());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->getAtlas());
    glUniform1i(atlas_location_, 0);
    glBindVertexArray(vao_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, message.size());
    glBindVertexArray(0u);
}
