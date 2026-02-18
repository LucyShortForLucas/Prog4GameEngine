#pragma once
#include <SDL3/SDL.h>
#include "Actor.h"
#include "Services.h"
#include <map>

struct SdlRendererDeleter {
	void operator()(SDL_Renderer* p) const {
		SDL_DestroyRenderer(p);
	}
};

namespace dae {
class Texture2D;
}

namespace eng {

class Renderer final : public service::IRenderer {
public:
	Renderer(SDL_Window* window);

	void Render(eng::Actor&) const override;

	void RenderTexture(const TextureMapping&) const override;

	void DelayRender(unsigned int layer, const TextureMapping&) override;

	SDL_Renderer* GetSDLRenderer() const override;

	const SDL_Color& GetBackgroundColor() const override { return m_clearColor; }
	void SetBackgroundColor(const SDL_Color& color) override { m_clearColor = color; }

private:
	mutable std::multimap<int, const TextureMapping> m_RenderLayerQueue{};
	std::unique_ptr<SDL_Renderer, SdlRendererDeleter> m_renderer{};
	SDL_Window* m_window{};
	SDL_Color m_clearColor{};
};
}

