#pragma once

#include "Texture2D.h"
#include <SDL3/SDL.h>

namespace eng {

struct TextureMapping final {
    const dae::Texture2D& texture;
    SDL_FRect screenRect;
    SDL_FRect TextureRect;
};

}