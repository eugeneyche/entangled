#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "board.hpp"
#include "font.hpp"
#include "text_renderer.hpp"
#include "board_renderer.hpp"

class Game
{
public:
    Game();

    void run();
    void quit();

    std::string title = "Entangled";
    int width = 800, height = 600;

    const uint8_t* keyboard_state;
    uint8_t mouse_state;
    int mouse_x;
    int mouse_y;

    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 camera_pos = glm::vec3 (0.0f, 0.0f, 30.0f);
    glm::vec3 camera_dir = glm::vec3 (0.0f, 0.0f, -1.0f);

    Board board;
    BoardRenderer* board_renderer = nullptr;

    Font* font = nullptr;
    TextRenderer* text_renderer = nullptr;

    bool is_running = false;
    SDL_Window* window = nullptr;

    uint32_t last_ticks = 0u;
    float delta_time = 0u;
    float fps = 0.0f;

    void initialize();
    void processEvents();
    void setup();
    void update();
    void destroy();
    void finalize();

    void drawFPS();
};
