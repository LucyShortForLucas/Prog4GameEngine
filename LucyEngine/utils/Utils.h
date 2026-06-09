#pragma once

#include "Texture2D.h"
#include <SDL3/SDL.h>
#include <chrono>
#include <cstdint>

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

struct Line2df final {
    glm::vec2 from;
    glm::vec2 to;
};

inline float LineRectIntersect(Line2df line, SDL_FRect rect) {
    float dx = line.from.x - line.to.x;
    float dy = line.from.y - line.to.y;

    float t0 = 0.0f, t1 = 1.0f;

    auto clip = [&](float p, float q) -> bool {
        if (std::abs(p) < 1e-8f) return q >= 0.0f;
        float r = q / p;
        if (p < 0.0f) t0 = std::max(t0, r);
        else          t1 = std::min(t1, r);
        return t0 <= t1;
        };

    if (!clip(-dx, line.from.x - rect.x))        return 1.0f;
    if (!clip(dx, rect.x + rect.w - line.from.x))   return 1.0f;
    if (!clip(-dy, line.from.y - rect.y))        return 1.0f;
    if (!clip(dy, rect.y + rect.h - line.from.y))   return 1.0f;
    if (t0 >= t1)                    return 1.0f;

    return t0;
}

}