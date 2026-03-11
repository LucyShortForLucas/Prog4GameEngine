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

	auto& thrashCache{ root->AddChildActor() };

	thrashCache.AddComponent<eng::ThrashTheCache>();

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

	//dae::Minigin engine(data_location);
	eng::Engine engine{data_location};
	engine.Run(load);
    return 0;
}
