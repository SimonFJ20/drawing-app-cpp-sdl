#include "result.hpp"
#include "sdl_graphics.hpp"
#include <iostream>
#include <string>

auto main() -> int
{
    auto grahics = sdl::Graphics::create();
    grahics->test();
    return 0;
}
