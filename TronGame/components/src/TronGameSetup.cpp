#include "TronGameSetup.h"

#include <Actor.h>
#include <SceneTree.h>
#include <Services.h>

#include "MoveTank.h"
#include "MoveTurret.h"
#include "ShootPlayerBullet.h"

#include <ColorMap.h>
#include <ActorJsonFactory.h>

#include "BuildLevel.h"


namespace tron {

std::unique_ptr<TronGameSetup> TronGameSetup::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<TronGameSetup>(owner);
}

void TronGameSetup::Start() {
	eng::service::IResourceLoader& resources{ eng::service::resources.Get() };
	
	Owner().DeserializeChild(BuildLevel("Background1.png", "Level1CollisonGrid.png"));
	auto player = Owner().DeserializeChild(*resources.LoadJson("Prefabs/PlayerTank.json"));
	player->GetTransform().SetGlobalPosition(100, 166);

	auto eney = Owner().DeserializeChild(*resources.LoadJson("Prefabs/EnemyTank.json"));
	eney->GetTransform().SetGlobalPosition(500, 166);

	auto eney2 = Owner().DeserializeChild(*resources.LoadJson("Prefabs/EnemyRecognizer.json"));
	eney2->GetTransform().SetGlobalPosition(500, 366);

		auto& input{eng::service::input.Get()};

	auto& inputGroup{ input.NewInputgroup(*player) };

	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_W }, std::make_unique<MoveTank>(glm::vec2{ 0, -120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_A }, std::make_unique<MoveTank>(glm::vec2{ -120, 0 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_S }, std::make_unique<MoveTank>(glm::vec2{ 0, 120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_D }, std::make_unique<MoveTank>(glm::vec2{ 120, 0 }));

	auto turret{ player->GetChildren().at(0) };

	auto& turretInputGroup{ input.NewInputgroup(*turret) };
	
	turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_E }, std::make_unique<MoveTurret>(true));
	turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_Q }, std::make_unique<MoveTurret>(false));
	turretInputGroup.SubscribeKeyDown(eng::Key{ SDL_SCANCODE_SPACE }, std::make_unique<ShootPlayerBullet>());
}

} // !tron