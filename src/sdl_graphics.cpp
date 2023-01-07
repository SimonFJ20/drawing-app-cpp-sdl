#include "sdl_graphics.hpp"
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <iostream>

namespace sdl {

[[nodiscard]] auto Graphics::create() -> Result<Graphics, void>
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return {};

    const auto width = 500;
    const auto height = 500;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) != 0)
        return {};
    return Graphics(window, renderer);
}

Graphics::~Graphics()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Graphics::test() { std::cout << "works\n"; }

}