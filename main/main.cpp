#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>

#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "logger.hpp"
#include "window.hpp"
#include "gameboy.hpp"
#include "cli.hpp"

using namespace Devices;

constexpr int WIDTH = 160;
constexpr int HEIGHT = 144;
constexpr uint8_t SCALE = 4;

bool init(SDL_Window **window, SDL_Surface **surface)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Could not initialize SDL video. Error: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Grown Up Boy!", WIDTH * SCALE, HEIGHT * SCALE, 0);

    if (*window == nullptr)
    {
        SDL_Log("Could not create SDL window. Error: %s", SDL_GetError());
        return false;
    }

    *surface = SDL_GetWindowSurface(*window);

    return true;
}

void close(SDL_Window **window, SDL_Surface **surface)
{
    SDL_DestroyWindow(*window);
    *window = nullptr;
    *surface = nullptr;
}

std::atomic_bool running(true);

void emulator_thread()
{
    while (running)
    {
    }
}

int main(int argc, char **argv)
{
    emulator::Profile profile;

    parseArgs(&profile, argc, argv);

    emulator::GameBoy gb(profile);

    return 0;
}