#pragma once
#include <string>
#include <functional>
#include <memory>
#include <filesystem>

#include "Services.h"

namespace fs = std::filesystem;

namespace eng {

class Engine {
public: //---------------|Constructor/destructor|---------------------------

	explicit Engine(const fs::path& dataPath);
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

public: //---------------|Run methods|---------------------------

	void Run(std::function<void()> loadGame);
	bool RunOneFrame(eng::Actor& root, eng::service::IRenderer& renderer);
};

}
