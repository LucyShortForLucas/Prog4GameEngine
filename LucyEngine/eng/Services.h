#pragma once
#include <string>
#include <memory>

#include "Utils.h"
#include "Actor.h"
#include "Font.h"
#include <chrono>

namespace eng::service {
//---------------------------------------- Service class ---------------------------------------
template <typename T>
class Service final {
public:
	Service(std::unique_ptr<T> defaultServiceUptr) : m_DefaultServiceUptr(std::move(defaultServiceUptr)), m_ServicePtr(m_DefaultServiceUptr.get()) {};

	void Register(std::unique_ptr<T> service) {
		m_ServicePtr = std::move(service);
	};

	void Unregister() {
		m_ServicePtr = m_DefaultServiceUptr;
	};

	T& Get() {
		return *m_ServicePtr;
	};
private:

	std::unique_ptr<T> m_DefaultServiceUptr{};
	std::unique_ptr<T> m_ServicePtr{};
};

//---------------------------------------- Services ---------------------------------------


class IRenderer {
public:
	virtual ~IRenderer() = default;

	virtual void Render(eng::Actor&) const = 0;
	virtual void RenderTexture(const TextureMapping&) const = 0;
	virtual void DelayRender(unsigned int layer, const TextureMapping&) = 0;

	virtual SDL_Renderer* GetSDLRenderer() const = 0;

	virtual const SDL_Color& GetBackgroundColor() const = 0;
	virtual void SetBackgroundColor(const SDL_Color& color) = 0;
};

extern Service<IRenderer> renderer;

class IResourceLoader {
public:
	virtual ~IResourceLoader() = default;

	virtual dae::Texture2D* LoadTexture(const std::string& file) = 0;
	virtual dae::Font* LoadFont(const std::string& file, uint8_t size) = 0;
};

extern Service<IResourceLoader>  resources;

class IGameTime {
public:
	virtual ~IGameTime() = default;

	virtual float	DeltaTime() const = 0;
	virtual void	UpdateDeltaTime() = 0;
	virtual int		MinMilliSecPerFrame() const = 0;
};

extern Service<IGameTime> gameTime;

}