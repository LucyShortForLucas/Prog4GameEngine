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
	//auto player{ Owner().GetSceneTree()->ClearAndLoad("Prefabs/PlayerTank.json") };

	//Owner().GetSceneTree()->LoadActor("Prefabs/PlayerTank.json")
	//	->GetTransform().SetGlobalPosition({200, 200});

	//auto& input{eng::service::input.Get()};

	//auto& inputGroup{ input.NewInputgroup(*player) };

	//inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_W }, std::make_unique<MoveTank>(glm::vec2{ 0, -120 }));
	//inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_A }, std::make_unique<MoveTank>(glm::vec2{ -120, 0 }));
	//inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_S }, std::make_unique<MoveTank>(glm::vec2{ 0, 120 }));
	//inputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_D }, std::make_unique<MoveTank>(glm::vec2{ 120, 0 }));

	//auto turret{ player->GetChildren().at(0) };

	//auto& turretInputGroup{ input.NewInputgroup(*turret) };
	//
	//turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_E }, std::make_unique<MoveTurret>(true));
	//turretInputGroup.SubscribeKeyPressed(eng::Key{ SDL_SCANCODE_Q }, std::make_unique<MoveTurret>(false));

	//eng::BlackWhiteMap<30, 28> bw{ *eng::service::resources.Get().LoadColorMap("Level1CollisonGrid.png") };

	//eng::ActorJsonFactory factory{};

	//nlohmann::ordered_json j{

	//factory
	//	.AddComponent("TextureRenderer")
	//		.AddProperty("TexturePath", "Background1.png")
	//		.AddProperty("Size", glm::vec2{ 968, 1032 })
	//		.Owner()
	//	.AddChildActor()
	//		.AddComponent("TextureRenderer")
	//			.AddProperty("TexturePath", "Tanks.png")
	//			.AddProperty("Size", glm::vec2{ 64, 64 })
	//			.AddProperty("SourceRect", SDL_FRect{0, 32, 32, 32})
	//			.Owner()
	//		.AddComponent("Transform")
	//			.AddProperty("LocalPosition", glm::vec2{200, 300})
	//			.Owner()
	//		.AddComponent("AabbCollider")
	//			.AddProperty("Bounds", SDL_FRect{0, 0, 64, 64})
	//			.Owner()
	//		.Parent()
	//	.AddChildActor()
	//		.AddComponent("Transform")
	//			.AddProperty("LocalPosition", glm::vec2{100, 100})
	//			.Owner()
	//		.AddComponent("TextureRenderer")
	//			.AddProperty("TexturePath", "Tanks.png")
	//			.AddProperty("Size", glm::vec2{ 64, 64 })
	//			.AddProperty("SourceRect", SDL_FRect{ 0, 0, 32, 32 })
	//			.Owner()
	//		.Parent()
	//	.Build()

	//};

	eng::service::IResourceLoader& resources{ eng::service::resources.Get() };
	
	Owner().DeserializeChild(BuildLevel("Background1.png", "Level1CollisonGrid.png"));
	auto player = Owner().DeserializeChild(*resources.LoadJson("Prefabs/PlayerTank.json"));
	player->GetTransform().SetGlobalPosition(100, 166);

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