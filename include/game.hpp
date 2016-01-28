#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "font.hpp"
#include "text_renderer.hpp"

class Game
{
public:
    Game();

    void run();
    void quit();

private:
    std::string title = "Entangled";
    int width = 800, height = 600;

    Font* font = nullptr;
    TextRenderer* text_renderer = nullptr;

    bool is_running = false;
    SDL_Window* window = nullptr;

    uint32_t last_ticks = 0u;
    float fps = 0.0f;

    void initialize();
    void processEvents();
    void setup();
    void update();
    void destroy();
    void finalize();
};
