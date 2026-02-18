#include "Services.h"
#include <SDL3/SDL_render.h>
#include "Font.h"

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

class NullResourceLoader final : public IResourceLoader {
public:
	virtual dae::Texture2D* LoadTexture(const std::string&) { return nullptr; };
	virtual dae::Font* LoadFont(const std::string&, uint8_t) { return nullptr; }
};

Service<IResourceLoader>  resources{ std::make_unique<NullResourceLoader>() };


class NullGameTime : public IGameTime {
public:
	virtual ~NullGameTime() = default;

	virtual float	DeltaTime() const { return 0.01f; };
	virtual void	UpdateDeltaTime() {};
	virtual int		MinMilliSecPerFrame() const { return 10; };
};

Service<IGameTime> gameTime{ std::make_unique<NullGameTime>() };

}