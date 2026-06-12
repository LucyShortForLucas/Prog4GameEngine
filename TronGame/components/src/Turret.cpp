#include "Turret.h"
#include <SceneTree.h>
#include <PhysicsBody.h>

namespace tron {

std::unique_ptr<Turret> Turret::Deserialize(eng::Actor& owner, const nlohmann::json&) {
	return std::make_unique<Turret>(owner);
}


void Turret::SetOutgoingBulletVelocity(glm::vec2 v) {
	m_OutgoingBulletVelocity = v;
}

void Turret::Start() {
	eng::SceneTree& tree{ *Owner().GetSceneTree() };

	for (int i{}; i++ < BULLET_POOL_SIZE;) {
		auto& bullet{ *tree.LoadActor("Prefabs/PlayerBullet.json") };
		bullet.EnableOnStart(false);
		tree.TagActor(tag::INACTIVE_BULLET, &bullet);
	}


	for (int i{}; i++ < SPARK_POOL_SIZE;) {
		auto& spark{ *tree.LoadActor("Prefabs/SmallSpark.json") };
		spark.EnableOnStart(false);
		tree.TagActor(tag::INACTIVE_SMALL_SPARK, &spark);
	}
}

void Turret::Shoot() {
	auto now{ std::chrono::steady_clock::now() };

	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_TimeOfLastShot).count() / 1000.f < m_MinSecondsBetweenShots)
		return;

	m_TimeOfLastShot = now;

	eng::SceneTree& tree{ *Owner().GetSceneTree() };

	auto bulletPtr{ tree.GetActorWithTag(tag::INACTIVE_BULLET) };

	if (bulletPtr == nullptr)
		return;

	glm::vec2 bulletSpawn{ Owner().GetParent()->GetTransform().GetGlobal().position };
	SDL_FRect bounds{ Owner().GetParent()->GetComponent<eng::AabbCollider>()->GetBounds()};

	glm::vec2 bulletdirection{ glm::normalize(m_OutgoingBulletVelocity) };

	bulletSpawn.x += bounds.w / 2 + bulletdirection.x * bounds.w;
	bulletSpawn.y += bounds.h / 2 + bulletdirection.y * bounds.h;

	bulletPtr->GetTransform().SetGlobalPosition(bulletSpawn);

	bulletPtr->GetComponent<eng::PhysicsBody>()->SetVelocity(m_OutgoingBulletVelocity);

	tree.UnTagActor(tag::INACTIVE_BULLET, bulletPtr);

	bulletPtr->Enable();
}

} // !tron