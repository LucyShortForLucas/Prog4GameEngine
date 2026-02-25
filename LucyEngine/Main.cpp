#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "Engine.h"
#include "LucyRenderer.h"
#include "Actor.h"
#include "TextureRenderer.h"
#include "FpsTracker.h"
#include "TextRenderer.h"
#include "ResourceLoader.h"
#include "Rotator.h"

#include <filesystem>
#include <memory>

#include "GameTime.h"

namespace fs = std::filesystem;

static std::unique_ptr<eng::Actor> load()
{
	//auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//auto go = std::make_unique<dae::GameObject>();
	//go->SetTexture("background.png");
	//scene.Add(std::move(go));

	//go = std::make_unique<dae::GameObject>();
	//go->SetTexture("logo.png");
	//go->SetPosition(358, 180);
	//scene.Add(std::move(go));

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_unique<dae::TextObject>("Programming 4 Assignment", font);
	//to->SetColor({ 255, 255, 0, 255 });
	//to->SetPosition(292, 20);
	//scene.Add(std::move(to));

	auto root{std::make_unique<eng::Actor>()};

	root->AddComponent<eng::TextureRenderer>("background.png");

	auto& logo{root->AddChildActor()};

	logo.AddComponent<eng::TextureRenderer>("logo.png");
	logo.GetComponent<eng::Transform>()->SetGlobalPosition(358, 180);

	auto& text{ root->AddChildActor() };

	text.AddComponent<eng::TextRenderer>("Test Test I am text");
	text.GetComponent<eng::Transform>()->SetGlobalPosition(20, 20);

	text.AddComponent<eng::FpsTracker>();

	auto& blueTank{ root->AddChildActor() };
	blueTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{32, 32}, SDL_FRect(0,0, 32, 32));
	blueTank.GetComponent<eng::Transform>()->SetGlobalPosition(400, 180);


	auto& redTank{ blueTank.AddChildActor() };
	redTank.AddComponent<eng::TextureRenderer>("tempTanks.png", glm::ivec2{ 32, 32 }, SDL_FRect(32, 0, 32, 32));
	redTank.GetComponent<eng::Transform>()->SetLocalPosition(64, 64);
	redTank.AddComponent<eng::Rotator>();

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
