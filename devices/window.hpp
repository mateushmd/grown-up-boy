#pragma once

#include <SDL3/SDL.h>

#include <string>

namespace Devices
{
    class Window
    {
    private:
        float scale{4};

        SDL_Window *window{nullptr};

        SDL_Surface *surface{nullptr};

        SDL_Surface *helloWorld{nullptr};

    public:
        bool init();
    };
}