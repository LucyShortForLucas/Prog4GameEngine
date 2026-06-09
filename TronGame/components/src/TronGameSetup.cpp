#include "TronGameSetup.h"

#include <Actor.h>
#include <SceneTree.h>
#include <Services.h>

#include "MovePhysicsBody.h"

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

	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_W }, std::make_unique<MovePhysicsBody>(glm::vec2{ 0, -120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_A }, std::make_unique<MovePhysicsBody>(glm::vec2{ -120, 0 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_S }, std::make_unique<MovePhysicsBody>(glm::vec2{ 0, 120 }));
	inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_D }, std::make_unique<MovePhysicsBody>(glm::vec2{ 120, 0 }));
}

} // !tron