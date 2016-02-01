#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "board_renderer.hpp"
#include "shader.hpp"

static float sqrt3_over_2 = 1.7320508075688772f / 2.0f;

TileRenderer::TileRenderer()
{
    std::vector<GLfloat> tile_vertex_buffer = {
         1.0f, 0.0f,          
         0.5f, sqrt3_over_2,  
        -0.5f, sqrt3_over_2, 
        -1.0f, 0.0f,         
        -0.5f, -sqrt3_over_2,
         0.5f, -sqrt3_over_2, 
    };

    std::vector<GLuint> tile_element_buffer = {
        0u, 1u, 2u,
        0u, 2u, 3u,
        0u, 3u, 4u,
        0u, 4u, 5u,
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

    tile_world_viewport_location_ = glGetUniformLocation(tile_program_, "world_viewport");
    glBindVertexArray(0u);

    // GLuint bezier_vert_shader = loadShader(GL_VERTEX_SHADER, "res/shader/bezier.vert");
    // GLuint bezier_frag_shader = loadShader(GL_FRAGMENT_SHADER, "res/shader/bezier.frag");
    // bezier_program_ = loadProgram({tile_vert_shader, berzier_frag_shader});
    // glDeleteShader(bezier_vert_shader);
    // glDeleteShader(bezier_frag_shader);
}

TileRenderer::~TileRenderer()
{
    glDeleteVertexArrays(1, &tile_vao_);
    glDeleteBuffers(1, &tile_vbo_);
    glDeleteBuffers(1, &tile_ebo_);
    glDeleteProgram(tile_program_);
}

void TileRenderer::draw(
        const glm::mat4& viewport,
        const Tile* tile,
        const glm::vec2& position,
        float rotation,
        float scale)
{
    glUseProgram(tile_program_);
    glBindVertexArray(tile_vao_);

    glm::mat4 world = 
        glm::translate(glm::mat4(1.0f), glm::vec3(position, 0)) *
        glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 0, 1)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(scale));
    glm::mat4 world_viewport = viewport * world;
    glUniformMatrix4fv(tile_world_viewport_location_, 1, GL_FALSE, 
            glm::value_ptr(world_viewport));
    glDrawElements(GL_TRIANGLES, tile_count_, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0u);
    glUseProgram(0u);
}


BoardRenderer::BoardRenderer() { }

BoardRenderer::~BoardRenderer() { }

void BoardRenderer::draw(
        const glm::mat4& viewport,
        const Board* board,
        const glm::vec2& position,
        float scale)
{
    static float rotation = 0.0f;
    for (int i = 0; i < board->getHeight(); i++)
    {
        for (int j = 0; j < board->getWidth(); j++)
        {
            glm::vec2 tile_position = position + BoardRenderer::getPosition(scale, i, j);
            if (i == 3 && j == 3)
            {
                tile_renderer_.draw(
                        viewport,
                        &board->getTile(i, j), 
                        tile_position, 
                        rotation, 
                        32);
            }
            else
            {
                tile_renderer_.draw(
                        viewport,
                        &board->getTile(i, j), 
                        tile_position, 
                        0.0f, 
                        32);
            }
        }
        rotation += 0.001f;
    }
}

glm::vec2 BoardRenderer::getPosition(float radius, int i, int j)
{
    glm::vec2 position;
    position[0] = 1.5f * radius * i + radius;
    position[1] = 2 * radius * sqrt3_over_2 * j + radius * sqrt3_over_2;
    if (i % 2)
    {
        position[1] += radius * sqrt3_over_2;
    }
    return position;
}
