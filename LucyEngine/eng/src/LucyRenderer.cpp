#include "LucyRenderer.h"

#include <stdexcept>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
#endif

    ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer.get());
    ImGui_ImplSDLRenderer3_Init(m_renderer.get());
}

eng::Renderer::~Renderer() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void eng::Renderer::Render(eng::Actor& root) const {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    root.RenderImgui();

    ImGui::EndFrame();

    ImGui::Render();
    
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
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer.get());
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
