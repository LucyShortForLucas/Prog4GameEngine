#pragma once

#include "Serialization.h"
#include "EventSource.h"
#include "AabbCollider.h"
#include "Transform2d.h"

namespace eng {

DECL_COMPONENT(PhysicsBody, public AbstractComponent, public AbstractEventListener<event::AabbCollisionEnter>)
public:
	//---- Helper Enums
	enum class BounceTypes {
																					/// Velocity drops to [0, 0] when hitting another physics body
		Stop,																		/// No change in velocity should occur when hitting another physics body
		Continue,																	/// Velocity should be reversed when hitting another physics body
		Bounce,																		/// Velocity should be reflected along the angle of the collision when hitting another physics body
		Reflect
	};
	
	//---- Ctor
	PhysicsBody(Actor& owner, glm::vec2 velocity, BounceTypes bounceType) : 
		AbstractComponent(owner), m_Velocity(velocity), m_BounceType(bounceType) {};

	//---- PhysicsBody Methods

	void SetVelocity(glm::vec2 velocity);
	void AddVelocity(glm::vec2 velocity);

	void SetBounceType(BounceTypes type);
	
	//---- Gameloop Methods
	void Start() override;
	void OnEnable() override;
	void OnDisable() override;
	void FixedUpdate() override;

	//---- Event Handler
	void OnEvent(const event::AabbCollisionEnter& context) override;

private:
	AabbCollider* m_ColliderPtr{};
	Line2df m_LastMovement{};
	glm::vec2 m_Velocity{};
	BounceTypes m_BounceType{BounceTypes::Stop};
};
REGISTER_COMPONENT(PhysicsBody)

NLOHMANN_JSON_SERIALIZE_ENUM(PhysicsBody::BounceTypes, {
	{PhysicsBody::BounceTypes::Stop, "Stop"},
	{PhysicsBody::BounceTypes::Continue, "Continue"},
	{PhysicsBody::BounceTypes::Bounce, "Bounce"},
	{PhysicsBody::BounceTypes::Reflect, "Reflect"},
	});

} // !eng