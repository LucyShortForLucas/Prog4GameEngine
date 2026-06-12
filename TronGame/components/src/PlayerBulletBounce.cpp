#include "PlayerBulletBounce.h"

#include <Actor.h>
#include <SceneTree.h>
#include <Turret.h>

namespace tron {

std::unique_ptr<PlayerBulletBounce> PlayerBulletBounce::Deserialize(eng::Actor& owner, const nlohmann::json& ) {
	return std::make_unique<PlayerBulletBounce>(owner);
}

void PlayerBulletBounce::OnEnable() {
	Owner().GetComponent<eng::PhysicsBody>()->SubscribePhysicsBodyBounced(*this);
}

void PlayerBulletBounce::OnDisable() {
	Owner().GetComponent<eng::PhysicsBody>()->UnsubscribePhysicsBodyBounced(*this);
}

void PlayerBulletBounce::OnEvent(const eng::event::PhysicsBodyBounced& context) {
	eng::SceneTree& tree{ *Owner().GetSceneTree() };

	auto spark{ tree.GetActorWithTag(tag::INACTIVE_SMALL_SPARK) };

	if (spark) {
		glm::vec2 pos{ Owner().GetTransform().GetGlobal().position };
		pos.y -= 8;
		pos.x -= 16;

		spark->GetTransform().SetGlobalPosition(pos);

		spark->Enable();

		tree.UnTagActor(tag::INACTIVE_SMALL_SPARK, spark);
	}

	++m_Bounces;

	if (m_Bounces <= 5)
		return;

	m_Bounces = 0;

	Owner().Disable();

	tree.TagActor(tag::INACTIVE_BULLET, &Owner());
}

} // !tron