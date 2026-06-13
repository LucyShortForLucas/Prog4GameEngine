#include "GameData.h"

#include <SceneTree.h>

#include <BuildLevel.h>
#include <Pathfinding.h>
#include <MoveTank.h>
#include <MoveTurret.h>
#include <ShootPlayerBullet.h>

namespace tron {

std::unique_ptr<GameData> GameData::Deserialize(eng::Actor& owner, const nlohmann::json& json) {
	return std::make_unique<GameData>(owner, json.value("LastHighscore", 1000));
}

void GameData::CreateActorsAndEnemies() {
	auto player = Owner().DeserializeChild(*eng::service::resources.Get().LoadJson("Prefabs/PlayerTank.json"));
	player->GetTransform().SetGlobalPosition(100, 166);

	auto pathfinding = Owner().GetSceneTree()->GetActorWithTag(eng::make_sdbm_hash("LevelHead"))->GetComponent<Pathfinding>();
	auto targetTileCenter{ pathfinding->GetGraph().Tile2x2ToPosition(pathfinding->GetGraph().GetRandomUnoccupied2x2(*Owner().GetSceneTree())) };
	player->GetTransform().SetGlobalPosition({ targetTileCenter.x - 32, targetTileCenter.y - 32 });

	auto eney = Owner().DeserializeChild(*eng::service::resources.Get().LoadJson("Prefabs/EnemyTank.json"));
	eney->GetTransform().SetGlobalPosition(500, 166);

	auto eney2 = Owner().DeserializeChild(*eng::service::resources.Get().LoadJson("Prefabs/EnemyRecognizer.json"));
	eney2->GetTransform().SetGlobalPosition(500, 366);

	auto& input{ eng::service::input.Get() };

	auto& inputGroup{ input.NewInputgroup(*player) };

	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_W }, std::make_unique<MoveTank>(glm::vec2{ 0, -120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_A }, std::make_unique<MoveTank>(glm::vec2{ -120, 0 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_S }, std::make_unique<MoveTank>(glm::vec2{ 0, 120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_D }, std::make_unique<MoveTank>(glm::vec2{ 120, 0 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Up }, std::make_unique<MoveTank>(glm::vec2{ 0, -120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Left }, std::make_unique<MoveTank>(glm::vec2{ -120, 0 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Down }, std::make_unique<MoveTank>(glm::vec2{ 0, 120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Right }, std::make_unique<MoveTank>(glm::vec2{ 120, 0 }));

	auto turret{ player->GetChildren().at(0) };

	auto& turretInputGroup{ input.NewInputgroup(*turret) };

	turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_E }, std::make_unique<MoveTurret>(true));
	turretInputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Lshoulder }, std::make_unique<MoveTurret>(true));
	turretInputGroup.SubscribeKeyPressed(eng::Key{ eng::GamepadKeys::Rshoulder }, std::make_unique<MoveTurret>(false));
	turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_Q }, std::make_unique<MoveTurret>(false));
	turretInputGroup.SubscribeKeyDown(eng::Key{ SDL_SCANCODE_SPACE }, std::make_unique<ShootPlayerBullet>());
	turretInputGroup.SubscribeKeyDown(eng::Key{ eng::GamepadKeys::A }, std::make_unique<ShootPlayerBullet>());
}

void GameData::Start() {
	Owner().PreserveOnParentClear();

	BuildLevel("Background1.png", "Level1CollisonGrid.png");

	CreateActorsAndEnemies();

	StartGame();
}

void GameData::NextLevel() {
	switch (m_LevelNum) {
	case 0:
		++m_LevelNum;

		BuildLevel("Background1.png", "Level1CollisonGrid.png");

		break;

	case 1:
		++m_LevelNum;

		BuildLevel("Background2.png", "Level2CollisionGrid.png");


		break;

	case 2:
		m_LevelNum = 0;

		BuildLevel("Background2.png", "Level2CollisionGrid.png");


		break;
	}
}

void GameData::Update() {
	//if (m_LevelNum >= 0 && Owner().GetSceneTree()->GetAllActorsWithTag(eng::make_sdbm_hash("Enemy")).empty()) {
	//	Owner().GetSceneTree()->ClearActors();

	//	NextLevel();
	//}
}

void GameData::StartGame() {
	m_LevelNum = 0;
}

void GameData::EndGame() {
	m_LevelNum = -1;
}

void GameData::OnEvent(const event::PlayerDied& context) {
	if (context.livesLeft > 0) {
		auto pathfinding = Owner().GetSceneTree()->GetActorWithTag(eng::make_sdbm_hash("LevelHead"))->GetComponent<Pathfinding>();

		auto targetTileCenter{ pathfinding->GetGraph().Tile2x2ToPosition(pathfinding->GetGraph().GetRandomUnoccupied2x2(*Owner().GetSceneTree())) };

		context.player.GetTransform().SetGlobalPosition({ targetTileCenter.x - 32, targetTileCenter.y - 32 });

		context.player.Enable();
	}
	else
		EndGame();
}


}