#include "EnemyController.h"

#include <Services.h>
#include <MoveTank.h>
#include <PlayerBulletBounce.h>

namespace tron {

std::unique_ptr<EnemyController> EnemyController::Deserialize(eng::Actor& owner, const nlohmann::json& json) {
	return std::make_unique<EnemyController>(owner, json.value("Speed", 100), json.value("Points", 100));
}

void EnemyController::Start() {
	m_Body = Owner().GetComponent<eng::PhysicsBody>();
}

void EnemyController::OnEnable() {
	Owner().GetComponent<eng::AabbCollider>()->SubscribeAabbCollisionEnter(*this);
}

void EnemyController::OnDisable() {
	Owner().GetComponent<eng::AabbCollider>()->UnsubscribeAabbCollisionEnter(*this);
}

void EnemyController::FixedUpdate() {

	auto& tree{ *Owner().GetSceneTree() };
	if (m_TimeUntilPathfind <= 0) {
		if (m_Pathfinding == nullptr)
			m_Pathfinding = tree.GetActorWithTag(eng::make_sdbm_hash("LevelHead"))->GetComponent<Pathfinding>();
		auto& playerPos{ tree.GetActorWithTag(eng::make_sdbm_hash("Player"))->GetTransform().GetGlobal().position };

		auto graph{ m_Pathfinding->GetGraph() };

		auto path{ graph.FindPath(graph.PositionToTile2x2(Owner().GetTransform().GetGlobal().position), graph.PositionToTile2x2(playerPos)) };

		if (!path.empty()) {
			auto direction{ path.at(0) };

			m_ShootVelocity = direction * 300.f;

			if (direction.x == 0) {
				if (direction.y > 0) 
					m_MoveDownCommand.Execute(Owner());				
				else
					m_MoveUpCommand.Execute(Owner());
			}
			else {
				if (direction.x > 0)
					m_MoveRightCommand.Execute(Owner());
				else
					m_MoveLeftCommand.Execute(Owner());
			}
		}

		m_TimeUntilPathfind = 0.1f;
	}
	else {
		m_TimeUntilPathfind -= eng::service::gameTime.Get().FixedDeltaTime();
	}

	if (m_TimeUntilShoot <= 0) {
		auto bullet{ tree.LoadActor("Prefabs/EnemyBullet.json") };

		bullet->GetComponent<eng::PhysicsBody>()->SetVelocity(m_ShootVelocity);
		bullet->GetTransform().SetGlobalPosition(Owner().GetTransform().GetGlobal().position + glm::vec2{30, 30});

		m_TimeUntilShoot = 2.2f;
	}
	else {
		m_TimeUntilShoot -= eng::service::gameTime.Get().FixedDeltaTime();
	}
}

void EnemyController::OnEvent(const eng::event::AabbCollisionEnter& context) {
	if (context.other.Owner().GetComponent<PlayerBulletBounce>()) {
		if (--m_HitPoints <= 0) {
			Owner().GetSceneTree()->UnTagActor(eng::make_sdbm_hash("Tank"), &Owner());
			Owner().Destroy();
			m_EnemyDiedEvent.Invoke({ m_PointWorth });
		}
	}
}

} // !tron