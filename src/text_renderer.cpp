#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "text_renderer.hpp"

TextRenderer::TextRenderer(int screen_width, int screen_height)
    : screen_width_ (screen_width)
    , screen_height_ (screen_height)
{
    GLuint vert_shader = loadShader(GL_VERTEX_SHADER, "res/shader/text.vert");
    GLuint frag_shader = loadShader(GL_FRAGMENT_SHADER, "res/shader/text.frag");
    program_ = loadProgram({vert_shader, frag_shader});
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    screen_width_location_ = glGetUniformLocation(program_, "screen_width");
    screen_height_location_ = glGetUniformLocation(program_, "screen_height");
    uv_rects_location_ = glGetUniformLocation(program_, "uv_rects");
    screen_rects_location_ = glGetUniformLocation(program_, "screen_rects");
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

void TextRenderer::draw(const std::string& message, int x, int y, Font* font)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<GLfloat> uv_rects;
    std::vector<GLfloat> screen_rects;
    int cursor_x = x;
    int cursor_y = y;
    for (size_t i = 0; i < message.size(); i++)
    {
        GlyphMetrics metrics = font->getGlyphMetrics(message[i]);
        GlyphUVRect rect = font->getGlyphUVRect(message[i]);

        uv_rects.push_back(rect.minu);
        uv_rects.push_back(rect.maxu);
        uv_rects.push_back(rect.minv);
        uv_rects.push_back(rect.maxv);

        screen_rects.push_back(cursor_x + metrics.minx);
        screen_rects.push_back(cursor_x + metrics.maxx);
        screen_rects.push_back(cursor_y + metrics.miny);
        screen_rects.push_back(cursor_y + metrics.maxy);

        cursor_x += metrics.advance;
    }
    glUseProgram(program_);
    glUniform1f(screen_width_location_, screen_width_);
    glUniform1f(screen_height_location_, screen_height_);
    glUniform4fv(uv_rects_location_, message.size(), uv_rects.data());
    glUniform4fv(screen_rects_location_, message.size(), screen_rects.data());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->getAtlas());
    glUniform1i(atlas_location_, 0);
    glBindVertexArray(vao_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, message.size());
    glBindVertexArray(0u);
}
