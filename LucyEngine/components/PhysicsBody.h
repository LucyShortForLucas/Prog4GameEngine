#pragma once

#include "Serialization.h"
#include "EventSource.h"
#include "AabbCollider.h"
#include "Transform2d.h"


namespace eng {

DECL_COMPONENT(PhysicsBody, public AbstractComponent, public AbstractEventListener<event::AabbCollisionEnter>)
public:
	//---- Ctor
	PhysicsBody(Actor& owner) : AbstractComponent(owner) {};

	//---- Gameloop Methods
	void OnEnable() override;
	void OnDisable() override;
	void FixedUpdate() override;

	//---- Event Handler
	void OnEvent(const event::AabbCollisionEnter& context) override;

private:
	eng::Line2df m_LastMovement;
	glm::vec2 m_Velocity;
};
REGISTER_COMPONENT(PhysicsBody)

} // !eng