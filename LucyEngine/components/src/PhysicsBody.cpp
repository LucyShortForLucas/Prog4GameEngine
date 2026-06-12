#include "PhysicsBody.h"
#include "Actor.h"
#include "AabbCollider.h"
#include "Services.h"

namespace eng {

std::unique_ptr<PhysicsBody> PhysicsBody::Deserialize(Actor& owner, const nlohmann::json& json) {
	return std::make_unique<PhysicsBody>(owner, json.value("Velocity", glm::vec2{}), json.value("BounceType", BounceTypes::Stop), json.value("VelocityMode", VelocityMode::Constant), json.value("Static", false));
}

nlohmann::ordered_json PhysicsBody::Serialize() {
	nlohmann::ordered_json j{};

	j["Velocity"] = m_Velocity;
	j["BounceType"] = m_BounceType;
	j["VelocityMode"] = m_VelocityMode;
	j["Static"] = m_Static;

	return j;
}


void PhysicsBody::Start() {
	m_ColliderPtr = Owner().GetComponent<AabbCollider>();
	if (m_ColliderPtr == nullptr)
		service::logger.Get().LogError("Physicsbody " + std::to_string((std::uintptr_t)this) + " does not have a collider.");
}

void PhysicsBody::OnEnable() {
	m_ColliderPtr->SubscribeAabbCollision(*this);
}

void PhysicsBody::OnDisable() {
	m_ColliderPtr->UnsubscribeAabbCollision(*this);
}

void PhysicsBody::FixedUpdate() {
	m_BouncedThisFrame = false;

	if (m_Velocity == glm::vec2{}) {
		m_LastMovement.from = m_LastMovement.to;
		return;
	}

	auto& transform{ Owner().GetTransform() };

	m_LastMovement.from = transform.GetGlobal().position;
	transform.TranslatePosition(m_Velocity * service::gameTime.Get().FixedDeltaTime());
	m_LastMovement.to = transform.GetGlobal().position;

	switch (m_VelocityMode) {
	case VelocityMode::Consume:
		m_Velocity = { 0,0 };
		break;
	}
}

void PhysicsBody::OnEvent(const event::AabbCollision& context) {
	auto otherBody{ context.other.Owner().GetComponent<PhysicsBody>() };
	if (!otherBody || m_Static || m_BouncedThisFrame)
		return; // No physics body in collison -> don't care

	m_BouncedThisFrame = true; // Only bounce once per frame

	// Apply bounce type to velocity
	switch (m_BounceType) {
	case eng::BounceTypes::Stop:
		m_Velocity = glm::vec2{};
		Owner().GetTransform().TranslatePosition((m_LastMovement.from - m_LastMovement.to));
		break;
	case eng::BounceTypes::Bounce:
		if (&context.other == m_LastPhysicsHitCollider)
			break;
		m_Velocity *= -1;
		break;
	case eng::BounceTypes::Reflect:
	{
		if (&context.other == m_LastPhysicsHitCollider) 
			break;
		SDL_FRect overlap{ m_ColliderPtr->OverlappingArea(context.other) };
		glm::vec2 reflectionAxis{ overlap.w < overlap.h ? glm::vec2{1, 0} : glm::vec2{0,1} };
		m_Velocity = glm::reflect(m_Velocity, reflectionAxis);
		break;
	}
	default:
		Owner().GetTransform().TranslatePosition((m_LastMovement.from - m_LastMovement.to));

		break;
	}

	m_LastPhysicsHitCollider = &context.other;
	m_PhysicsBodyBouncedEvent.Invoke({ this, otherBody, m_BounceType});
}

void PhysicsBody::SetVelocity(glm::vec2 velocity) {
	m_Velocity = velocity;
}

void PhysicsBody::AddVelocity(glm::vec2 velocity) {
	m_Velocity += velocity;
}

void PhysicsBody::SetBounceType(BounceTypes type) {
	m_BounceType = type;
}

glm::vec2 PhysicsBody::GetVelocity() {
	return m_Velocity;
}

} // !eng