#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "board.hpp"

class BoardRenderer
{
public:
    BoardRenderer();
    ~BoardRenderer();

    void draw(const glm::mat4& projection, const glm::mat4& view, Board* board);

private:
    GLuint tile_vao_ = 0u;
    GLuint tile_vbo_ = 0u;
    GLuint tile_ebo_ = 0u;
    GLint tile_count_ = 0u;

    GLuint tile_program_ = 0u;
    GLuint tile_world_view_location_ = 0u;
    GLuint tile_it_world_view_location_ = 0u;
    GLuint tile_world_view_projection_location_ = 0u;

    GLuint bezier_program_ = 0u;
    GLuint bezier_vao_ = 0u;
    GLuint bezier_vbo_ = 0u;

    GLuint bezier_world_view_projection_location_ = 0u;
    GLuint bezier_begin_control_location_ = 0u;
    GLuint bezier_end_control_location_ = 0u;

    glm::vec2 getPosition(int x, int y) const;
};
