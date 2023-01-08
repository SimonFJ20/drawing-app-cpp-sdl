#pragma once

#include "result.hpp"
#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <vector>

namespace sdl {

struct Color final {
    static constexpr const auto alpha_transparent = 0;
    static constexpr const auto alpha_opaque = 255;

    uint8_t red {}, green {}, blue {}, alpha = alpha_opaque;
};

struct Rectangle final {
    int x, y, width, height;

    [[nodiscard]] auto as_sdl_rect() noexcept -> SDL_Rect*
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return reinterpret_cast<SDL_Rect*>(this);
    }
};

class Event final {
public:
    Event(SDL_Event&& sdl_event)
        : sdl_event { std::forward<SDL_Event>(sdl_event) }
    { }

    [[nodiscard]] constexpr auto is_exit() const noexcept
    {
        return sdl_event.type == SDL_QUIT;
    }

    [[nodiscard]] constexpr auto window_id() const noexcept
        -> std::optional<uint32_t>;

private:
    SDL_Event sdl_event;
};

struct MouseSnapshot final {
public:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    MouseSnapshot(int x, int y, bool left_pressed, bool middle_pressed,
        bool right_pressed)
        : x_location { x }
        , y_location { y }
        , buttons_pressed {
            left_pressed,
            middle_pressed,
            right_pressed,
        }
    { }

    [[nodiscard]] auto x() const noexcept { return x_location; }
    [[nodiscard]] auto y() const noexcept { return y_location; }
    [[nodiscard]] auto left_pressed() const noexcept
    {
        return buttons_pressed[0];
    }
    [[nodiscard]] auto middle_pressed() const noexcept
    {
        return buttons_pressed[1];
    }
    [[nodiscard]] auto right_pressed() const noexcept
    {
        return buttons_pressed[2];
    }

private:
    enum class Buttons : uint8_t { Left = 0, Middle = 1, Right = 2 };

    int x_location, y_location;
    std::vector<bool> buttons_pressed;
};

class Graphics final {
public:
    Graphics(SDL_Window* window, SDL_Renderer* renderer);
    [[nodiscard]] Graphics(const Graphics&) = delete;
    auto operator=(const Graphics&) -> Graphics& = delete;
    Graphics(Graphics&& moved) = delete;
    auto operator=(Graphics&&) -> Graphics& = delete;
    ~Graphics();
    static auto create(int width, int height) noexcept
        -> Result<std::unique_ptr<Graphics>, void>;
    [[nodiscard]] auto poll_events() noexcept -> std::vector<Event>;
    auto present() noexcept -> void;
    auto clear(Color color) noexcept -> void;
    auto fill_rectangle(Rectangle rectangle, Color color) noexcept -> void;

    [[nodiscard]] auto mouse_snapshot() const noexcept -> MouseSnapshot;

private:
    auto set_color(Color color) noexcept -> void;

    SDL_Window* window;
    SDL_Renderer* renderer;
};

}