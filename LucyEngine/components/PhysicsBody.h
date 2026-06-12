#pragma once

#include "Serialization.h"
#include "EventSource.h"
#include "AabbCollider.h"
#include "Transform2d.h"

namespace eng {

enum class BounceTypes : int {														/// Velocity drops to [0, 0] when hitting another physics body
	Stop,																		/// No change in velocity should occur when hitting another physics body
	Continue,																	/// Velocity should be reversed when hitting another physics body
	Bounce,																		/// Velocity should be reflected along the angle of the collision when hitting another physics body
	Reflect
};

class PhysicsBody;

namespace event {

struct PhysicsBodyBounced {
	const PhysicsBody* const bouncedBody;
	const PhysicsBody* const otherBody;
	const BounceTypes bounceType;
};

} // !event

DECL_COMPONENT(PhysicsBody, public AbstractComponent, public AbstractEventListener<event::AabbCollision>)
	DECL_EVENT(PhysicsBodyBounced)
public:
	//---- Helper Enums

	enum class VelocityMode : int {														/// Velocity is constant, only automatically affected by 'bouncing'.
		Constant,																	/// Velocity is 'consumed' every frame. After moving the physicsbody, its velocity is set to 0
		Consume
	};
	
	//---- Ctor
	PhysicsBody(Actor& owner, glm::vec2 velocity, BounceTypes bounceType, VelocityMode velocityMode, bool isStatic) : 
		AbstractComponent(owner), m_Velocity(velocity), m_BounceType(bounceType), m_VelocityMode(velocityMode), m_Static{isStatic} {};

	nlohmann::ordered_json Serialize() override;

	//---- PhysicsBody Methods

	void SetVelocity(glm::vec2 velocity);
	void AddVelocity(glm::vec2 velocity);

	void SetBounceType(BounceTypes type);

	glm::vec2 GetVelocity();
	
	//---- Gameloop Methods
	void Start() override;
	void OnEnable() override;
	void OnDisable() override;
	void FixedUpdate() override;

	//---- Event Handler
	void OnEvent(const event::AabbCollision& context) override;

private:
	AabbCollider* m_ColliderPtr{};
	AabbCollider* m_LastPhysicsHitCollider{};
	Line2df m_LastMovement{};
	glm::vec2 m_Velocity{};
	BounceTypes m_BounceType{ BounceTypes::Stop };
	VelocityMode m_VelocityMode{ VelocityMode::Constant};
	bool m_Static{ false };
	bool m_BouncedThisFrame{false};
};
REGISTER_COMPONENT(PhysicsBody)

NLOHMANN_JSON_SERIALIZE_ENUM(BounceTypes, {
	{BounceTypes::Stop, "Stop"},
	{BounceTypes::Continue, "Continue"},
	{BounceTypes::Bounce, "Bounce"},
	{BounceTypes::Reflect, "Reflect"},
	});

NLOHMANN_JSON_SERIALIZE_ENUM(PhysicsBody::VelocityMode, {
	{PhysicsBody::VelocityMode::Constant, "Constant"},
	{PhysicsBody::VelocityMode::Consume, "Consume"}
	});

} // !eng

/*

{
	"Type": "PhysicsBody"
	"Json": {
		"Velocity": [0,0],
		"BounceType": "Stop"
	}
}

*/