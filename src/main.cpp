#include "result.hpp"
#include "sdl_graphics.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cstddef>
#include <fmt/core.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class Brush {
public:
    Brush(std::vector<sdl::Color>& canvas, int width, int height)
        : canvas { canvas }
        , width { width }
        , height { height }
    { }

    auto paint(int x, int y, sdl::Color color) -> void
    {
        set_pixel(x, y, color);
    }

private:
    auto set_pixel(int x, int y, sdl::Color color) -> void
    {
        canvas[y * height + x] = color;
    }

    std::vector<sdl::Color>& canvas;
    [[maybe_unused]] int width, height;
};

auto main() -> int
{
    const auto width = 160L;
    const auto height = 160L;
    const auto pixel_width = 5;
    const auto pixel_height = 5;

    auto canvas = std::vector<sdl::Color>(height * width);
    for (auto& pixel : canvas)
        pixel = { 255, 255, 255 };

    auto brush = Brush(canvas, width, height);

    auto graphics = std::move(
        sdl::Graphics::create(width * pixel_width, height * pixel_height)
            .unwrap());

    while (true) {
        auto events = graphics->poll_events();
        if (std::ranges::any_of(
                events, [](auto& event) { return event.is_exit(); }))
            break;

        graphics->clear({ 60, 60, 60 });

        auto snapshot = graphics->mouse_snapshot();
        if (snapshot.left_pressed()) {
            brush.paint(snapshot.x() / pixel_width,
                (snapshot.y() / pixel_height), { 0, 0, 0 });
        }

        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                graphics->fill_rectangle(
                    {
                        x * pixel_width,
                        y * pixel_height,
                        pixel_width,
                        pixel_height,
                    },
                    canvas.at(height * y + x));

        graphics->present();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 0;
}
