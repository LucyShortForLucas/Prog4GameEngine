#pragma once
#include <string>
#include <functional>
#include <memory>
#include <filesystem>

#include "Services.h"

namespace fs = std::filesystem;

namespace eng {

class Engine {
public: //--------------- Constructor/destructor ---------------------------

	explicit Engine(const fs::path& dataPath);
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

public: //--------------- Run methods ---------------------------

	void Run(std::function<std::unique_ptr<eng::Actor>()> loadGame);
	bool RunOneFrame();

private: //----------------- Scene data -------------------

	std::unique_ptr<Actor> m_RootActor;
};

}
