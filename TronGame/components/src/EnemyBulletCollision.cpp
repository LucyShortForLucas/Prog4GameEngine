#include "EnemyBulletCollisions.h"

namespace tron {

std::unique_ptr<EnemyBulletCollisions> EnemyBulletCollisions::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<EnemyBulletCollisions>(owner);
}

void EnemyBulletCollisions::OnEvent(const eng::event::AabbCollisionEnter& context) {
	Owner().Destroy();
}

void EnemyBulletCollisions::OnEnable() {
	Owner().GetComponent<eng::AabbCollider>()->SubscribeAabbCollisionEnter(*this);

}

void EnemyBulletCollisions::OnDisable() {
	Owner().GetComponent<eng::AabbCollider>()->UnsubscribeAabbCollisionEnter(*this);

}

}