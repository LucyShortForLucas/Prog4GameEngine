#include "EnemyController.h"

#include <Services.h>

namespace tron {

std::unique_ptr<EnemyController> EnemyController::Deserialize(eng::Actor& owner, const nlohmann::json& json) {
	return std::make_unique<EnemyController>(owner, json.value("Speed", 100));
}

void EnemyController::Start() {
	m_Body = Owner().GetComponent<eng::PhysicsBody>();
}

void EnemyController::FixedUpdate() {
	auto& tree{ *Owner().GetSceneTree() };
	if (m_TimeUntilPathfind <= 0) {
		if (m_Pathfinding == nullptr)
			m_Pathfinding = tree.GetActorWithTag(eng::make_sdbm_hash("LevelHead"))->GetComponent<Pathfinding>();
		auto& playerPos{ tree.GetActorWithTag(eng::make_sdbm_hash("Player"))->GetTransform().GetGlobal().position };

		auto graph{ m_Pathfinding->GetGraph() };

		auto path{ graph.FindPath(graph.PositionToTile2x2(Owner().GetTransform().GetGlobal().position), graph.PositionToTile2x2(playerPos)) };



		if (!path.empty())
			m_Body->SetVelocity(path.at(0) * m_Speed );

		m_TimeUntilPathfind = 0.1f;
	}
	else {
		m_TimeUntilPathfind -= eng::service::gameTime.Get().FixedDeltaTime();
	}
}

} // !tron