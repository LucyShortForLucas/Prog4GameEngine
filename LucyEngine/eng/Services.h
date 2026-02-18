#pragma once
#include <string>
#include <memory>

#include "Utils.h"
#include "Actor.h"

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
	std::unique_ptr<T> m_ServicePtr;
	std::unique_ptr<T> m_DefaultServiceUptr;
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

}