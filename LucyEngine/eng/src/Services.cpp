#include "Services.h"
#include <SDL3/SDL_render.h>

namespace eng::service {

class NullRenderer final : public IRenderer {
public:
	virtual void Render(eng::Actor&) const {}

	virtual void RenderTexture(const TextureMapping&) const {}

	virtual SDL_Renderer* GetSDLRenderer() const { return nullptr; }

	virtual void DelayRender(unsigned int, const TextureMapping&) {};

	virtual const SDL_Color& GetBackgroundColor() const { return m_Color; }
	virtual void SetBackgroundColor(const SDL_Color&) {}
private:
	SDL_Color m_Color{};
};

Service<IRenderer> renderer{ std::make_unique<NullRenderer>() };


}