#include "LucyRenderer.h"

#include <stdexcept>
#include "Texture2D.h"
#include "Utils.h"

eng::Renderer::Renderer(SDL_Window* window) {
    m_window = window;

    // Let SDL3 choose best renderer
    m_renderer.reset(SDL_CreateRenderer(window, nullptr));

    if (!m_renderer) {
        throw std::runtime_error(
            std::string("SDL_CreateRenderer Error: ") + SDL_GetError()
        );
    }
}

void eng::Renderer::Render(eng::Actor& root) const {
    const auto& color = GetBackgroundColor();

    SDL_SetRenderDrawColor(
        m_renderer.get(),
        color.r, color.g, color.b, color.a
    );

    SDL_RenderClear(m_renderer.get());

    root.Render();

    for (auto& [key, mapping] : m_RenderLayerQueue) {
        RenderTexture(mapping);
    }

    m_RenderLayerQueue.clear();

    SDL_RenderPresent(m_renderer.get());
}

void eng::Renderer::RenderTexture(const TextureMapping& mapping) const {

    SDL_RenderTexture(
        GetSDLRenderer(),
        mapping.texture.GetSDLTexture(),
        &mapping.TextureRect,
        &mapping.screenRect
    );
}

void eng::Renderer::DelayRender(unsigned int layer, const TextureMapping& mapping) {

    m_RenderLayerQueue.emplace(layer, mapping);
}

SDL_Renderer* eng::Renderer::GetSDLRenderer() const {
    return m_renderer.get();
}
