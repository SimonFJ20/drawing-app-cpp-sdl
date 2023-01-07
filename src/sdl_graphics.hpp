#pragma once

#include "result.hpp"
#include <SDL_render.h>
#include <SDL_video.h>

namespace sdl {

class Graphics {
public:
    [[nodiscard]] static auto create() -> Result<Graphics, void>;
    ~Graphics();
    void test();

private:
    Graphics(SDL_Window* window, SDL_Renderer* renderer)
        : window(window)
        , renderer(renderer)
    {
    }
    SDL_Window* window;
    SDL_Renderer* renderer;
};

}