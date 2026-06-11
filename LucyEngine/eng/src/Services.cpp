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
	virtual dae::Texture2D* const LoadTexture(const std::string&) { return nullptr; };
	virtual dae::Font* const LoadFont(const std::string&, uint8_t) { return nullptr; }
	virtual nlohmann::json* const LoadJson(const std::string&) { return nullptr; }
	virtual ColorMap* const LoadColorMap(const std::string&) { return nullptr; }
};

Service<IResourceLoader>  resources{ std::make_unique<NullResourceLoader>() };


class NullGameTime : public IGameTime {
public:
	virtual ~NullGameTime() = default;

	virtual float	DeltaTime() const { return 0.01f; };
	virtual float	FixedDeltaTime() const { return 0.01f; };
	virtual void	UpdateDeltaTime() {};
	virtual bool	UpdateFixedDeltaTime() { return false; };
	virtual int		MinMilliSecPerFrame() const { return 10; };
};

Service<IGameTime> gameTime{ std::make_unique<NullGameTime>() };


class NullInput final : public IInput {
public:
	bool ProcessInput() override { return false; }
	CommandInputGroup& NewInputgroup(Actor&) override { return m_NullInputGroup; }
	EventSource<event::KeyUp>& UpEventSource() { return m_NullKeyUpEvent; };
	EventSource<event::KeyPressed>& PressedEventSource() { return m_NullKeyPressedEvent; };
	EventSource<event::KeyDown>& DownEventSource() { return m_NullKeyDownEvent; };

private:
	CommandInputGroup m_NullInputGroup{ nullptr };
	EventSource<event::KeyUp> m_NullKeyUpEvent;
	EventSource<event::KeyPressed> m_NullKeyPressedEvent;
	EventSource<event::KeyDown> m_NullKeyDownEvent;
};

Service<IInput> input{ std::make_unique<NullInput>() };

extern Service<IInput> input;

class NullAudioPlayer final : public IAudioPlayer {
public:
	virtual void PlaySound(const HashedString& sound) {};
	virtual void StopSound(int) {};
	virtual void StopSound() {};
};

Service<IAudioPlayer> audioPlayer{ std::make_unique<NullAudioPlayer>() };

class NullLogger final : public ILogger {
public:
	void Log(const std::string& text) override {};
	void LogError(const std::string& text) override {};
	void LogSuccess(const std::string& text) override {};

};

Service<ILogger> logger{ std::make_unique<NullLogger>() };

}