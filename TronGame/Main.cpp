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
#include "PhysicsBody.h"
#include <SpriteAnimator.h>
#include <ColorMap.h>
#include <ActorJsonFactory.h>

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

	eng::Engine engine{ "Tron - Battle Tanks - Lucas Schonkeren", 968, 1032 };
	eng::service::audioPlayer.Register(std::make_unique<eng::AudioPlayer>()); // We set up the audo after the engine because it relies on SDL, which is initialized in the engine
	std::string file{ "TronGameStartup.json" };
	engine.Run(file);

	eng::service::audioPlayer.Unregister(); // Manually unregistering the SDL Mixer service to ensure it is destroyed before a call to SDL_Quit
	return 0;
}
