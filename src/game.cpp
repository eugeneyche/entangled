#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include "error.hpp"
#include "game.hpp"
#include "shader.hpp"

Game::Game() { }

void Game::run()
{
    initialize();
    setup();
    is_running = true;
    while (is_running)
    {
        uint32_t ticks = SDL_GetTicks();
        uint32_t delta_ticks = ticks - last_ticks;
        if (delta_ticks > 0)
        {
            fps = (fps + (1000.0f / delta_ticks)) / 2;
        }
        last_ticks = ticks;
        processEvents();
        update();
        SDL_GL_SwapWindow(window);
    }
    finalize();
}

void Game::quit()
{
    is_running = false;
}

void Game::initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fatalError("Failed to initialize SDL.");
    }
    if (TTF_Init() < 0)
    {
        fatalError("Failed to initialize SDL TTF.");
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL);
    if (!window)
    {
        fatalError("Failed to create window.");
    }
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fatalError("Failed to create context.");
    }
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fatalError("Failed to initialize GLEW.");
    }
    glGetError();
}

void Game::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit();
        }
    }
}

void Game::setup()
{
    glViewport(0, 0, width, height);

    font = new Font("res/font/gamegirl.ttf", 18);
    text_renderer = new TextRenderer(width, height);
}

    static int count = 0;

void Game::update()
{
    glClear(GL_COLOR_BUFFER_BIT);
    count++;
    text_renderer->draw("fps: " + std::to_string(int(fps)), 10, 10, font);
}

void Game::destroy()
{
    delete font;
    delete text_renderer;
}

void Game::finalize()
{
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
