#pragma once

#include <Serialization.h>
#include <AbstractComponent.h>
#include <Actor.h>

namespace tron {

constexpr int BULLET_POOL_SIZE{ 32 };

constexpr int SPARK_POOL_SIZE{ 64 };

namespace tag {
constexpr unsigned int INACTIVE_BULLET{ eng::make_sdbm_hash("InactiveBullet") };
constexpr unsigned int INACTIVE_SMALL_SPARK{ eng::make_sdbm_hash("InactiveSmallSpark") };

}

DECL_COMPONENT(Turret, public eng::AbstractComponent)
public:
	//---- Ctor
	Turret(eng::Actor& owner) : AbstractComponent(owner) {};

	//---- Setters
	void SetOutgoingBulletVelocity(glm::vec2 v);

	//---- Turret methods
	void Shoot();

	//---- Gameloop methods
	void Start();

private:
	glm::vec2 m_OutgoingBulletVelocity{ glm::vec2{160, 0} };
	const float m_MinSecondsBetweenShots{0.2f};
	std::chrono::steady_clock::time_point m_TimeOfLastShot{};
};
REGISTER_COMPONENT(Turret)
}