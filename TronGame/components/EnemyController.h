#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <Pathfinding.h>
#include <PhysicsBody.h>
#include <MoveTank.h>
#include <EventSource.h>

namespace tron {

namespace event {

struct EnemyDied {
	int pointWorth;
};

}

DECL_COMPONENT(EnemyController, public eng::AbstractComponent, public eng::AbstractEventListener<eng::event::AabbCollisionEnter>)
	DECL_EVENT(EnemyDied)
public:
	//---- Ctor

	EnemyController(eng::Actor& owner, float speed, int points) : eng::AbstractComponent(owner), m_Speed(speed),
		m_MoveDownCommand{ glm::vec2{0, speed} },
		m_MoveUpCommand{ glm::vec2{0, -speed} },
		m_MoveRightCommand{ glm::vec2{speed, 0} },
		m_MoveLeftCommand{ glm::vec2{-speed, 0} },
		m_PointWorth(points)
		{
	};

//---- Gameloop Methods 
	void Start() override;
	void OnEnable() override;
	void OnDisable() override;
	void FixedUpdate() override;

//---- Event Handlers

	void OnEvent(const eng::event::AabbCollisionEnter& context);

private:
	int m_HitPoints{ 3 };
	int m_PointWorth{ 100 };
	float m_TimeUntilPathfind{ -1 };
	float m_TimeUntilShoot{0.7f};
	float m_Speed{};

	Pathfinding* m_Pathfinding{};
	eng::PhysicsBody* m_Body{};

	MoveTank m_MoveUpCommand;
	MoveTank m_MoveDownCommand;
	MoveTank m_MoveLeftCommand;
	MoveTank m_MoveRightCommand;

	glm::vec2 m_ShootVelocity;

}; // !EnemyController
REGISTER_COMPONENT(EnemyController)

} // !tron