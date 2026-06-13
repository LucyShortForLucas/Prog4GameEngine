#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <Pathfinding.h>
#include <PhysicsBody.h>

namespace tron {

DECL_COMPONENT(EnemyController, public eng::AbstractComponent)
public:
	//---- Ctor

	EnemyController(eng::Actor& owner, float speed) : eng::AbstractComponent(owner), m_Speed(speed) {};

//---- Gameloop Methods 
	void Start() override;
	void FixedUpdate() override;

private:
	int m_HitPoints{ 3 };
	float m_TimeUntilPathfind{-1};
	float m_Speed{};

	Pathfinding* m_Pathfinding{};
	eng::PhysicsBody* m_Body{};


}; // !EnemyController
REGISTER_COMPONENT(EnemyController)

} // !tron