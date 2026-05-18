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
#include "Kill.h"
#include "AddScore.h"
#include "HpScore.h"
#include "HpScoreDisplay.h"
#include "Logger.h"
#include "CommandInputGroup.h"
#include "AudioPlayer.h"

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

	auto& blueTankUi{ root->AddChildActor() };
	
	blueTankUi.GetComponent<eng::Transform>()->SetGlobalPosition(10, 300);
	blueTankUi.AddComponent<eng::TextRenderer>("# lives: 3 Score: 0");
	blueTankUi.AddComponent<eng::HpScoreDisplay>();

	auto& blueTankControlText{ root->AddChildActor() };
	blueTankControlText.AddComponent<eng::TextRenderer>(
		"Use WASD to move the blue tank, C to inflict damage, Z and X to get score");
	blueTankControlText.GetComponent<eng::Transform>()->SetGlobalPosition(10, 100);
	
	auto& blueTank{ root->AddChildActor() };

	blueTank.AddComponent<eng::TextureRenderer>("tempTanks.png",
		glm::ivec2{ 32, 32 }, SDL_FRect{ 0, 0, 32, 32 });

	blueTank.AddComponent<eng::HpScore>(3)
		.SubscribeLives(*blueTankUi.GetComponent<eng::HpScoreDisplay>());

	blueTank.GetComponent<eng::Transform>()->SetGlobalPosition(400, 300);

	auto& input{ eng::service::input.Get() };

	auto& blueTankInput{ input.NewInputgroup(blueTank) };

	auto& inputService{ eng::service::input.Get() };

	eng::CommandInputGroup::KeyEventSources keyEventSources {
		inputService.DownEventSource(),
		inputService.PressedEventSource(),
		inputService.UpEventSource()
	};

	blueTankInput.SubscribeInputSource(keyEventSources);

	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_W, std::make_unique<eng::Move>(glm::vec2{ 0, -80 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_A, std::make_unique<eng::Move>(glm::vec2{ -80, 0 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_S, std::make_unique<eng::Move>(glm::vec2{ 0, 80 }));
	blueTankInput.SubscribeKeyPressed(SDL_SCANCODE_D, std::make_unique<eng::Move>(glm::vec2{ 80, 0 }));

	blueTankInput.SubscribeKeyDown(SDL_SCANCODE_C, std::make_unique<eng::Kill>());
	blueTankInput.SubscribeKeyDown(SDL_SCANCODE_Z, std::make_unique<eng::AddScore>(10));
	blueTankInput.SubscribeKeyDown(SDL_SCANCODE_X, std::make_unique<eng::AddScore>(100));

	auto& redTankUi{ root->AddChildActor() };

	redTankUi.GetComponent<eng::Transform>()->SetGlobalPosition(10, 400);
	redTankUi.AddComponent<eng::TextRenderer>("# lives: 3 Score: 0");
	redTankUi.AddComponent<eng::HpScoreDisplay>();

	auto& redTankControlText{ root->AddChildActor() };
	redTankControlText.AddComponent<eng::TextRenderer>("Use the D-pad to move the blue tank, X to inflict damage, A and B to get score");
	redTankControlText.GetComponent<eng::Transform>()->SetGlobalPosition(10, 150);

	auto& redTank{ root->AddChildActor() };

	redTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{ 32, 32 }, SDL_FRect{ 32, 0, 32, 32 });
	redTank.AddComponent<eng::HpScore>(3).SubscribeLives(*redTankUi.GetComponent<eng::HpScoreDisplay>());
	redTank.GetComponent<eng::Transform>()->SetGlobalPosition(500, 300);

	auto& redTankInput{ input.NewInputgroup(redTank) };
	redTankInput.SubscribeInputSource(keyEventSources);

	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Up, std::make_unique<eng::Move>(glm::vec2{ 0, -80 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Left, std::make_unique<eng::Move>(glm::vec2{ -80, 0 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Down, std::make_unique<eng::Move>(glm::vec2{ 0, 80 }));
	redTankInput.SubscribeKeyPressed(eng::GamepadKeys::Right, std::make_unique<eng::Move>(glm::vec2{ 80, 0 }));

	redTankInput.SubscribeKeyDown(eng::GamepadKeys::X, std::make_unique<eng::Kill>());
	redTankInput.SubscribeKeyDown(eng::GamepadKeys::A, std::make_unique<eng::AddScore>(10));
	redTankInput.SubscribeKeyDown(eng::GamepadKeys::B, std::make_unique<eng::AddScore>(100));


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

	#ifndef NDEBUG // Use Assert Logger in debug mode, and messageboxLogger in release
		eng::service::logger.Register(std::make_unique<eng::AssertLogger>());
	#else
		eng::service::logger.Register(std::make_unique<eng::MessageBoxLogger>());
	#endif

	eng::service::resources.Register(std::make_unique<eng::SdlResourceLoader>());
	eng::service::gameTime.Register(std::make_unique<eng::GameTime>());
	eng::service::input.Register(std::make_unique<eng::Input>());

	eng::Engine engine{data_location};
	eng::service::audioPlayer.Register(std::make_unique<eng::AudioPlayer>()); // We set up the audo after the engine because it relies on SDL, which is initialized in the engine
	engine.Run(load);
	eng::service::audioPlayer.Unregister(); // Manually unregistering the SDL Mixer service to ensure it is destroyed before a call to SDL_Quit
    return 0;
}
