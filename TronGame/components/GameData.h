#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

#include <PlayerLives.h>

namespace tron {

DECL_COMPONENT(GameData, public eng::AbstractComponent, public eng::AbstractEventListener<event::PlayerDied>)
public:
	//---- Ctor

	GameData(eng::Actor& owner, int previousHighScore) : eng::AbstractComponent(owner), m_PreviousHighScores(previousHighScore) {};

//---- Gameloop Methods 
	void Start() override;
	void Update() override;

//---- API
	void StartGame();
	void EndGame();
	void NextLevel();
	void CreateActorsAndEnemies();

//---- Event handlers

	void OnEvent(const event::PlayerDied&);

private:
	int m_Score{};
	int m_PreviousHighScores{1000};
	float m_PlayerRessurectTimer{};
	int m_LevelNum{-1};

}; // !FpsTracker
REGISTER_COMPONENT(GameData)

} // !tron