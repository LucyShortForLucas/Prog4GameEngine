#include "TeleportToRand2x2.h"

#include <services.h>
#include <Actor.h>

namespace tron {

std::unique_ptr<TeleportToRand2x2> TeleportToRand2x2::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<TeleportToRand2x2>(owner);
}

void TeleportToRand2x2::Start() {}

void TeleportToRand2x2::OnEnable() {
	Owner().GetComponent<eng::AabbCollider>()->SubscribeAabbCollisionEnter(*this);
}

void TeleportToRand2x2::OnDisable() {
	Owner().GetComponent<eng::AabbCollider>()->UnsubscribeAabbCollisionEnter(*this);
}

void TeleportToRand2x2::OnEvent(const eng::event::AabbCollisionEnter& context) {
	m_Pathfinding = Owner().GetSceneTree()->GetActorWithTag(eng::make_sdbm_hash("LevelHead"))->GetComponent<Pathfinding>();

	auto targetTileCenter{ m_Pathfinding->GetGraph().Tile2x2ToPosition(m_Pathfinding->GetGraph().GetRandomUnoccupied2x2(*Owner().GetSceneTree())) };

	context.other.Owner().GetTransform().SetGlobalPosition({ targetTileCenter.x-32, targetTileCenter.y-32});
}

} // !tron