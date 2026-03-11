#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif


#include "Engine.h"
#include "LucyRenderer.h"
#include "Actor.h"
#include "TextureRenderer.h"
#include "FpsTracker.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Rotator.h"
#include "ThrashTheCache.h"

#include <filesystem>
#include <memory>

#include "GameTime.h"
#include "Input.h"
#include "Services.h"
#include "Move.h"

namespace fs = std::filesystem;

static std::unique_ptr<eng::Actor> load()
{
	auto root{std::make_unique<eng::Actor>()};

	root->AddComponent<eng::TextureRenderer>("background.png");

	auto& logo{root->AddChildActor()};

	logo.AddComponent<eng::TextureRenderer>("logo.png");
	logo.GetComponent<eng::Transform>()->SetGlobalPosition(358, 180);

	auto& fps{ root->AddChildActor() };

	fps.AddComponent<eng::TextRenderer>("Text");
	fps.GetComponent<eng::Transform>()->SetGlobalPosition(20, 20);
	fps.AddComponent<eng::FpsTracker>();

	auto& blueTank{ root->AddChildActor() };

	blueTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{ 32, 32 }, SDL_FRect{0, 0, 32, 32});
	blueTank.GetComponent<eng::Transform>()->SetGlobalPosition(200, 100);

	auto& input{ eng::service::input.Get() };

	auto& blueTankInput{ input.NewInputgroup(blueTank) };
	blueTankInput.SubscribeInputSource(input.GetEventSource());

	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_W, std::make_unique<eng::Move>(glm::vec2{ 0, -80 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_A, std::make_unique<eng::Move>(glm::vec2{ -80, 0 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_S, std::make_unique<eng::Move>(glm::vec2{ 0, 80 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_D, std::make_unique<eng::Move>(glm::vec2{ 80, 0 }));

	auto& redTank{ root->AddChildActor() };

	redTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{ 32, 32 }, SDL_FRect{ 32, 0, 32, 32 });
	redTank.GetComponent<eng::Transform>()->SetGlobalPosition(200, 150);

	auto& redTankInput{ input.NewInputgroup(redTank) };
	redTankInput.SubscribeInputSource(input.GetEventSource());

	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Up, std::make_unique<eng::Move>(glm::vec2{ 0, -80 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Left, std::make_unique<eng::Move>(glm::vec2{ -80, 0 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Down, std::make_unique<eng::Move>(glm::vec2{ 0, 80 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Right, std::make_unique<eng::Move>(glm::vec2{ 80, 0 }));


	return root;
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	// Set up services
	eng::service::resources.Register(std::make_unique<eng::SdlResourceLoader>());
	eng::service::gameTime.Register(std::make_unique<eng::GameTime>());
	eng::service::input.Register(std::make_unique<eng::Input>());

	//dae::Minigin engine(data_location);
	eng::Engine engine{data_location};
	engine.Run(load);
    return 0;
}
