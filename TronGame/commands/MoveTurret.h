#pragma once

#include <AbstractCommand.h>
#include <SpriteAnimator.h>
#include <Turret.h>

namespace tron {

class MoveTurret : public eng::AbstractCommand {
public:
	//---- Ctor
	MoveTurret(bool clockwise, float minSecDelay = 0.1f, float bulletSpeed = 400);

	//---- Dtor
	~MoveTurret() override;

	//---- Command methods
	bool Execute(eng::Actor& actor) override;

private:
	//---- State fields
	const bool m_Clockwise{};
	const float m_MinSecDelay{};
	const float m_BulletSpeed{};
	float m_SecondsPassed{};

	//---- Caching
	static inline std::map<eng::Actor*, eng::SpriteAnimator*> s_ActorAnimators{};
	static inline std::map<eng::Actor*, tron::Turret*> s_ActorTurrets{};
};

}