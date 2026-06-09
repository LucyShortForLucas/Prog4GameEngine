#pragma once
#include <string>
#include <functional>
#include <memory>
#include <filesystem>

#include "Services.h"
#include "SceneTree.h"

namespace fs = std::filesystem;

namespace eng {

class Engine {
public: //--------------- Constructor/destructor ---------------------------

	explicit Engine(const std::string& title, int width, int height);
	~Engine();

	Engine(const Engine& other) = delete;
	Engine(Engine&& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine& operator=(Engine&& other) = delete;

public: //--------------- Run methods ---------------------------

	void Run(const std::string& initalActorPath);
	bool RunOneFrame();

private: //----------------- Scene data -------------------

	std::unique_ptr<SceneTree> m_SceneTreeUptr;
};

}
