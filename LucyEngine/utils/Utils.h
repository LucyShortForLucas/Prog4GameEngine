#pragma once

#include "Texture2D.h"
#include <SDL3/SDL.h>
#include <chrono>

namespace eng {

struct TextureMapping final {
    const dae::Texture2D& texture;
    SDL_FRect screenRect;
    SDL_FRect TextureRect;
};

inline std::chrono::zoned_seconds ZonedSecondsNow() {
    auto now = std::chrono::system_clock::now();
    auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
    return std::chrono::zoned_time(std::chrono::current_zone(), sec);
}

}