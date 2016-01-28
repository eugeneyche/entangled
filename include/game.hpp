#pragma once

#include <SDL2/SDL.h>

class Game
{
public:
    Game();

    void run();
    void quit();

private:
    bool is_running_ = false;
    SDL_Window* window_ = nullptr;

    void initialize();
    void processEvents();
    void setup();
    void update();
    void finalize();

};
