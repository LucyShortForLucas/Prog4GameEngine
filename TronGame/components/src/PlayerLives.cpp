#include <PlayerLives.h>

#include <EnemyBulletCollisions.h>

namespace tron {

std::unique_ptr<PlayerLives> PlayerLives::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<PlayerLives>(owner);
}

void PlayerLives::OnEvent(const eng::event::AabbCollisionEnter& context) {
	if (context.other.Owner().GetComponent<EnemyBulletCollisions>() == nullptr)
		return;

	//--m_Lives; <-- Disabled for testing purposes

	Owner().Disable();

	m_PlayerDiedEvent.Invoke({ Owner(), m_Lives });
}

void PlayerLives::OnEnable() {
	Owner().GetComponent<eng::AabbCollider>()->SubscribeAabbCollisionEnter(*this);
}

void PlayerLives::OnDisable() {
	Owner().GetComponent<eng::AabbCollider>()->UnsubscribeAabbCollisionEnter(*this);
}

}