#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "board_renderer.hpp"
#include "shader.hpp"

BoardRenderer::BoardRenderer() 
{ 
    float sqrt3 = 1.7320508075688772f;
    float sqrt3_over_2 = sqrt3 / 2;

    float depth = 0.1f;

    std::vector<GLfloat> tile_vertex_buffer = {
        1.0f, 0.0f,           depth,
        0.5f, sqrt3_over_2,   depth,
        -0.5f, sqrt3_over_2,  depth,
        -1.0f, 0.0f,          depth,
        -0.5f, -sqrt3_over_2, depth,
        0.5f, -sqrt3_over_2,  depth,
        1.0f, 0.0f,           -depth,
        0.5f, sqrt3_over_2,   -depth,
        -0.5f, sqrt3_over_2,  -depth,
        -1.0f, 0.0f,          -depth,
        -0.5f, -sqrt3_over_2, -depth,
        0.5f, -sqrt3_over_2,  -depth,
    };

    std::vector<GLuint> tile_element_buffer = {
        0u, 1u, 2u,
        0u, 2u, 3u,
        0u, 3u, 4u,
        0u, 4u, 5u,

        1u, 6u, 7u,
        1u, 7u, 2u,

        2u, 7u, 8u,
        2u, 8u, 3u,

        3u, 8u, 9u,
        3u, 9u, 4u,

        4u, 9u, 10u,
        4u, 10u, 5u,

        5u, 10u, 11u,
        5u, 11u, 6u,
        
        6u, 11u, 12u,
        6u, 12u, 7u,

        6u, 11u, 10u,
        6u, 10u, 9u,
        6u, 9u, 8u,
        6u, 8u, 7u,
    };

    tile_count_ = tile_element_buffer.size();

    glGenVertexArrays(1, &tile_vao_);
    glGenBuffers(1, &tile_vbo_);
    glGenBuffers(1, &tile_ebo_);

    glBindVertexArray(tile_vao_);

    glBindBuffer(GL_ARRAY_BUFFER, tile_vbo_);
    glBufferData(
            GL_ARRAY_BUFFER, 
            sizeof(GLfloat) * tile_vertex_buffer.size(),
            tile_vertex_buffer.data(),
            GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tile_ebo_);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(GLuint) * tile_element_buffer.size(),
            tile_element_buffer.data(),
            GL_STATIC_DRAW);

    GLuint tile_vert_shader = loadShader(GL_VERTEX_SHADER, "res/shader/tile.vert");
    GLuint tile_frag_shader = loadShader(GL_FRAGMENT_SHADER, "res/shader/tile.frag");
    tile_program_ = loadProgram({tile_vert_shader, tile_frag_shader});
    glDeleteShader(tile_vert_shader);
    glDeleteShader(tile_frag_shader);

    tile_world_view_projection_location_ = glGetUniformLocation(tile_program_, "world_view_projection");
    glBindVertexArray(0u);
}

BoardRenderer::~BoardRenderer()
{
    glDeleteVertexArrays(1, &tile_vao_);
    glDeleteBuffers(1, &tile_vbo_);
    glDeleteBuffers(1, &tile_ebo_);
    glDeleteProgram(tile_program_);
}

void BoardRenderer::draw(
        const glm::mat4& view_projection, 
        Board* board)
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram(tile_program_);
    glBindVertexArray(tile_vao_);
    glUniformMatrix4fv(tile_world_view_projection_location_, 1, GL_FALSE, glm::value_ptr(view_projection));
    glDrawElements(GL_TRIANGLES, tile_count_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0u);
}
