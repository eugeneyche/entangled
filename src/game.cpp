#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "error.hpp"
#include "game.hpp"

Game::Game() { }

void Game::run()
{
    initialize();
    setup();
    is_running_ = true;
    while (is_running_)
    {
        processEvents();
        update();
        SDL_GL_SwapWindow(window_);
    }
    finalize();
}

void Game::quit()
{
    is_running_ = false;
}

void Game::initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fatalError("Failed to initialize SDL.");
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    window_ = SDL_CreateWindow(
            "Entangled",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800,
            600,
            SDL_WINDOW_OPENGL);
    if (!window_)
    {
        fatalError("Failed to create window.");
    }
    SDL_GLContext context = SDL_GL_CreateContext(window_);
    if (!context)
    {
        fatalError("Failed to create context.");
    }
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fatalError("Failed to initialize GLEW.");
    }
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
}

void Game::update()
{
}

void Game::finalize()
{
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
