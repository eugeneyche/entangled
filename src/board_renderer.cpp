#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "board_renderer.hpp"
#include "shader.hpp"

static float sqrt3_over_2 = 1.7320508075688772f / 2;

BoardRenderer::BoardRenderer() 
{ 
    float depth = 100.0f;

    std::vector<GLfloat> tile_vertex_buffer = {
        1.0f, 0.0f,           0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, sqrt3_over_2,   0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, sqrt3_over_2,  0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f,          0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -sqrt3_over_2, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -sqrt3_over_2,  0.0f, 0.0f, 0.0f, 1.0f,

        1.0f, 0.0f,           -depth, 0.0f, 0.0f, -1.0f,
        0.5f, sqrt3_over_2,   -depth, 0.0f, 0.0f, -1.0f,
        -0.5f, sqrt3_over_2,  -depth, 0.0f, 0.0f, -1.0f,
        -1.0f, 0.0f,          -depth, 0.0f, 0.0f, -1.0f,
        -0.5f, -sqrt3_over_2, -depth, 0.0f, 0.0f, -1.0f,
        0.5f, -sqrt3_over_2,  -depth, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f,           0.0f, 0.5f, sqrt3_over_2, 0.0f,
        0.5f, sqrt3_over_2,   0.0f, 0.5f, sqrt3_over_2, 0.0f,
        1.0f, 0.0f,           -depth, 0.5f, sqrt3_over_2, 0.0f,
        0.5f, sqrt3_over_2,   -depth, 0.5f, sqrt3_over_2, 0.0f,

        0.5f, sqrt3_over_2,   0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, sqrt3_over_2,  0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, sqrt3_over_2,   -depth, 0.0f, 1.0f, 0.0f,
        -0.5f, sqrt3_over_2,  -depth, 0.0f, 1.0f, 0.0f,

        -0.5f, sqrt3_over_2,  0.0f, -0.5f, sqrt3_over_2, 0.0f,
        -1.0f, 0.0f,          0.0f, -0.5f, sqrt3_over_2, 0.0f,
        -0.5f, sqrt3_over_2,  -depth, -0.5f, sqrt3_over_2, 0.0f,
        -1.0f, 0.0f,          -depth, -0.5f, sqrt3_over_2, 0.0f,

        -1.0f, 0.0f,          0.0f, -0.5f, -sqrt3_over_2, 0.0f,
        -0.5f, -sqrt3_over_2, 0.0f, -0.5f, -sqrt3_over_2, 0.0f,
        -1.0f, 0.0f,          -depth, -0.5f, -sqrt3_over_2, 0.0f,
        -0.5f, -sqrt3_over_2, -depth, -0.5f, -sqrt3_over_2, 0.0f,

        -0.5f, -sqrt3_over_2, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, -sqrt3_over_2,  0.0f, 0.0f, -1.0f, 0.0f,
        -0.5f, -sqrt3_over_2, -depth, 0.0f, -1.0f, 0.0f,
        0.5f, -sqrt3_over_2,  -depth, 0.0f, -1.0f, 0.0f,

        0.5f, -sqrt3_over_2,  0.0f, 0.5f, -sqrt3_over_2, 0.0f,
        1.0f, 0.0f,           0.0f, 0.5f, -sqrt3_over_2, 0.0f,
        0.5f, -sqrt3_over_2,  -depth, 0.5f, -sqrt3_over_2, 0.0f,
        1.0f, 0.0f,           -depth, 0.5f, -sqrt3_over_2, 0.0f
    };

    std::vector<GLuint> tile_element_buffer = {
        0u, 1u, 2u,
        0u, 2u, 3u,
        0u, 3u, 4u,
        0u, 4u, 5u,

        6u, 11u, 10u,
        6u, 10u, 9u,
        6u, 9u, 8u,
        6u, 8u, 7u,

        12u, 14u, 15u,
        12u, 15u, 13u,

        16u, 18u, 19u,
        16u, 19u, 17u,

        20u, 22u, 23u,
        20u, 23u, 21u,

        24u, 26u, 27u,
        24u, 27u, 25u,

        28u, 30u, 31u,
        28u, 31u, 29u,

        32u, 34u, 35u,
        32u, 35u, 33u,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(GLfloat) * 6, 
            reinterpret_cast<GLvoid*>(sizeof(GLfloat) * 3));

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

    tile_it_world_view_location_ = glGetUniformLocation(tile_program_, "world_view");
    tile_it_world_view_location_ = glGetUniformLocation(tile_program_, "it_world_view");
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
        const glm::mat4& projection, 
        const glm::mat4& view, 
        Board* board)
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glUseProgram(tile_program_);
    glBindVertexArray(tile_vao_);
    for (int i = 0; i < board->getHeight(); i++)
    {
        for (int j = 0; j < board->getWidth(); j++)
        {
            glm::vec2 position = getPosition(i, j);
            glm::mat4 world_view = glm::translate(view, glm::vec3(position, 1));
            glm::mat4 it_world_view = glm::transpose(glm::inverse(world_view));
            glm::mat4 world_view_projection = projection * world_view;
            glUniformMatrix4fv(tile_world_view_location_, 1, GL_FALSE, glm::value_ptr(world_view));
            glUniformMatrix4fv(tile_it_world_view_location_, 1, GL_FALSE, glm::value_ptr(it_world_view));
            glUniformMatrix4fv(tile_world_view_projection_location_, 1, GL_FALSE, glm::value_ptr(world_view_projection));
            glDrawElements(GL_TRIANGLES, tile_count_, GL_UNSIGNED_INT, 0);
        }
    }
    glBindVertexArray(0u);
}

glm::vec2 BoardRenderer::getPosition(int x, int y) const
{
    float radius = 1.2f;
    glm::vec2 position;
    position[0] = 1.5f * radius * x;
    position[1] = 2 * radius * sqrt3_over_2 * y;
    if (x % 2)
    {
        position[1] += radius * sqrt3_over_2;
    }
    return position;
}
