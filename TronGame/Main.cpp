#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Engine.h"
#include "LucyRenderer.h"
#include "Actor.h"
#include "TextureRenderer.h"
#include "FpsTracker.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"

#include <filesystem>
#include <memory>

#include "GameTime.h"
#include "Input.h"
#include "Services.h"
#include "Logger.h"
#include "CommandInputGroup.h"
#include "AudioPlayer.h"

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>

/// This has to be here because otherwise VLD shits the bed. (VLD sees a singleton within the STL chrono library as like a billion leaks)
namespace {
static inline bool _ = [] {
	VLDDisable();
	[[maybe_unused]] volatile auto _ = std::chrono::current_zone();
	VLDEnable();
	return true;
	}();

}
#endif



namespace fs = std::filesystem;

//static void load()
//{
//	auto root{std::make_unique<eng::Actor>()};
//
//	root->AddComponent<eng::TextureRenderer>("background.png");
//
//	auto& logo{root->AddChildActor()};
//
//	logo.AddComponent<eng::TextureRenderer>("logo.png");
//	logo.GetComponent<eng::Transform>()->SetGlobalPosition(358, 180);
//
//	auto& fps{ root->AddChildActor() };
//
//	fps.AddComponent<eng::TextRenderer>("Text");
//	fps.GetComponent<eng::Transform>()->SetGlobalPosition(20, 20);
//	fps.AddComponent<eng::FpsTracker>();
//
//	auto& blueTankUi{ root->AddChildActor() };
//	
//	blueTankUi.GetComponent<eng::Transform>()->SetGlobalPosition(10, 300);
//	blueTankUi.AddComponent<eng::TextRenderer>("# lives: 3 Score: 0");
//
//	auto& blueTankControlText{ root->AddChildActor() };
//	blueTankControlText.AddComponent<eng::TextRenderer>(
//		"Use WASD to move the blue tank, C to inflict damage, Z and X to get score");
//	blueTankControlText.GetComponent<eng::Transform>()->SetGlobalPosition(10, 100);
//	
//	auto& blueTank{ root->AddChildActor() };
//
//	blueTank.AddComponent<eng::TextureRenderer>("tempTanks.png",
//		glm::ivec2{ 32, 32 }, SDL_FRect{ 0, 0, 32, 32 });
//
//
//	blueTank.GetComponent<eng::Transform>()->SetGlobalPosition(400, 300);
//
//	auto& input{ eng::service::input.Get() };
//
//	auto& blueTankInput{ input.NewInputgroup(blueTank) };
//
//	auto& inputService{ eng::service::input.Get() };
//
//	eng::CommandInputGroup::KeyEventSources keyEventSources {
//		inputService.DownEventSource(),
//		inputService.PressedEventSource(),
//		inputService.UpEventSource()
//	};
//
//	blueTankInput.SubscribeInputSource(keyEventSources);
//
//	auto& redTankUi{ root->AddChildActor() };
//
//	redTankUi.GetComponent<eng::Transform>()->SetGlobalPosition(10, 400);
//	redTankUi.AddComponent<eng::TextRenderer>("# lives: 3 Score: 0");
//
//	auto& redTankControlText{ root->AddChildActor() };
//	redTankControlText.AddComponent<eng::TextRenderer>("Use the D-pad to move the blue tank, X to inflict damage, A and B to get score");
//	redTankControlText.GetComponent<eng::Transform>()->SetGlobalPosition(10, 150);
//
//	auto& redTank{ root->AddChildActor() };
//
//	redTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{ 32, 32 }, SDL_FRect{ 32, 0, 32, 32 });
//	redTank.GetComponent<eng::Transform>()->SetGlobalPosition(500, 300);
//
//	auto& redTankInput{ input.NewInputgroup(redTank) };
//	
//	root->Serialize("C:\\Lucy\\DAE\\Programming\\Prog4\\RetakeYear\\Prog4GameEngine\\TronGame\\Data\\Json\\Out\\Demo.json");
//}

int main(int, char*[]) {
	#if _DEBUG && __has_include(<vld.h>)
	VLDSetOptions(VLD_OPT_SKIP_CRTSTARTUP_LEAKS, 1, 0);
	#endif

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
	std::string file{ "Demo.json" };
	engine.Run(file);

	eng::service::audioPlayer.Unregister(); // Manually unregistering the SDL Mixer service to ensure it is destroyed before a call to SDL_Quit
	return 0;
}
