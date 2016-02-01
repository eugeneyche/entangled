#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "board.hpp"

class TileRenderer
{
public:
    TileRenderer();
    ~TileRenderer();

    void draw(
            const glm::mat4& viewport,
            const Tile* tile,
            const glm::vec2& position,
            float rotation,
            float scale);

private:
    GLuint tile_vao_ = 0u;
    GLuint tile_vbo_ = 0u;
    GLuint tile_ebo_ = 0u;
    GLint tile_count_ = 0u;

    GLuint tile_program_ = 0u;
    GLuint tile_world_viewport_location_ = 0u;

    GLuint bezier_program_ = 0u;
    GLuint bezier_vao_ = 0u;
    GLuint bezier_vbo_ = 0u;

    GLuint bezier_world_view_projection_location_ = 0u;
    GLuint bezier_begin_control_location_ = 0u;
    GLuint bezier_end_control_location_ = 0u;
};

class BoardRenderer
{
public:
    BoardRenderer();
    ~BoardRenderer();

    void draw(
            const glm::mat4& viewport,
            const Board* board,
            const glm::vec2& position,
            float scale);
private:
    TileRenderer tile_renderer_;

    static glm::vec2 getPosition(float radius, int i, int j);
};
