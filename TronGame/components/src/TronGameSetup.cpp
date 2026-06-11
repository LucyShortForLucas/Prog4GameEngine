#include "TronGameSetup.h"

#include <Actor.h>
#include <SceneTree.h>
#include <Services.h>

#include "MoveTank.h"
#include "MoveTurret.h"

#include <ColorMap.h>


namespace tron {

std::unique_ptr<TronGameSetup> TronGameSetup::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<TronGameSetup>(owner);
}

void TronGameSetup::Start() {
	auto player{ Owner().GetSceneTree()->ClearAndLoad("Prefabs/PlayerTank.json") };

	Owner().GetSceneTree()->LoadActor("Prefabs/PlayerTank.json")
		->GetTransform().SetGlobalPosition({200, 200});

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

	eng::BlackWhiteMap<30, 28> bw{ *eng::service::resources.Get().LoadColorMap("Level1CollisonGrid.png") };
}

} // !tron