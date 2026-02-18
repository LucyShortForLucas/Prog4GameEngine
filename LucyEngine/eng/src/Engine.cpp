#include "Engine.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <thread>
#include "Services.h"
#include "InputManager.h"
#include "Actor.h"
#include "LucyRenderer.h"

#include "AbstractComponent.h"


static SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch) {
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"


void LoopCallback(void* arg) {
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion() {
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
}

eng::Engine::Engine(const fs::path&) {
	PrintSDLVersion();

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) {
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	// Set up renderer from window as service
	eng::service::renderer.Register(std::make_unique<eng::Renderer>(g_window));
}

eng::Engine::~Engine() {
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void eng::Engine::Run(std::function<std::unique_ptr<eng::Actor>()> load) {
#ifndef __EMSCRIPTEN__
	// Set up services
	auto& renderer = eng::service::renderer.Get(); //TODO: make service

	std::unique_ptr<eng::Actor> f_Root{std::move(load())};

	// Set up time
	auto lastTime = std::chrono::high_resolution_clock::now();

	bool doContinue = true;
	while (doContinue) {
		lastTime = std::chrono::high_resolution_clock::now();
		doContinue = RunOneFrame(*f_Root, renderer);
		std::this_thread::sleep_for(lastTime + std::chrono::milliseconds(16) - std::chrono::high_resolution_clock::now());
	}
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

bool eng::Engine::RunOneFrame(eng::Actor& root, eng::service::IRenderer& renderer) {
	// Fetch time
	auto& gameTime{ service::gameTime.Get() };
	gameTime.UpdateDeltaTime();
		
	root.Start();

	bool doContinue{ dae::InputManager::GetInstance().ProcessInput() };

	root.Update();

	root.LateUpdate();

	renderer.Render(root);

	return doContinue;
}