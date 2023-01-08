#include "sdl_graphics.hpp"
#include "SDL_mouse.h"
#include "result.hpp"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>

namespace sdl {

[[nodiscard]] constexpr auto Event::window_id() const noexcept
    -> std::optional<uint32_t>
{
    switch (sdl_event.type) {
        case SDL_WINDOWEVENT:
            return sdl_event.window.windowID;
        case SDL_QUIT:
            return {};
        default:
            std::cerr << "unexhaustive enum match: value \"" << sdl_event.type
                      << "\" not matched in function "
                      << static_cast<const char*>(__func__) << "\n";
            std::abort();
    }
}

Graphics::Graphics(SDL_Window* window, SDL_Renderer* renderer)
    : window(window)
    , renderer(renderer)
{ }

Graphics::~Graphics()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

[[nodiscard]] auto Graphics::create(int width, int height) noexcept
    -> Result<std::unique_ptr<Graphics>, void>
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return {};

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) != 0)
        return {};
    return std::make_unique<Graphics>(window, renderer);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
[[nodiscard]] auto Graphics::poll_events() noexcept -> std::vector<Event>
{
    auto events = std::vector<Event> {};
    while (SDL_PollEvent(nullptr) != 0) {
        auto sdl_event = SDL_Event {};
        SDL_PollEvent(&sdl_event);
        events.emplace_back(std::move(sdl_event));
    }
    return events;
}

auto Graphics::present() noexcept -> void { SDL_RenderPresent(renderer); }

auto Graphics::clear(Color color) noexcept -> void
{
    set_color(color);
    SDL_RenderClear(renderer);
}

auto Graphics::fill_rectangle(Rectangle rectangle, Color color) noexcept -> void
{
    set_color(color);
    SDL_RenderFillRect(renderer, rectangle.as_sdl_rect());
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
[[nodiscard]] auto Graphics::mouse_snapshot() const noexcept -> MouseSnapshot
{
    int x = 0;
    int y = 0;
    auto bitmask = SDL_GetMouseState(&x, &y);
    return {
        x,
        y,
        (bitmask & SDL_BUTTON(1)) != 0U,
        (bitmask & SDL_BUTTON(2)) != 0U,
        (bitmask & SDL_BUTTON(3)) != 0U,
    };
}

auto Graphics::set_color(Color color) noexcept -> void
{
    SDL_SetRenderDrawColor(
        renderer, color.red, color.green, color.blue, color.alpha);
}

}