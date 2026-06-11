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
	m_ColliderPtr->SubscribeAabbCollisionEnter(*this);
}

void PhysicsBody::OnDisable() {
	m_ColliderPtr->UnsubscribeAabbCollisionEnter(*this);
}

void PhysicsBody::FixedUpdate() {
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

void PhysicsBody::OnEvent(const event::AabbCollisionEnter& context) {
	if (!context.other.Owner().GetComponent<PhysicsBody>() || m_Static)
		return; // No physics body in collison -> don't care

	// Apply bounce type to velocity
	switch (m_BounceType) {
	case eng::PhysicsBody::BounceTypes::Stop:
		m_Velocity = glm::vec2{};
		break;
	case eng::PhysicsBody::BounceTypes::Bounce:
		m_Velocity *= -1;
		break;
	case eng::PhysicsBody::BounceTypes::Reflect:
	{
		SDL_FRect overlap{ m_ColliderPtr->OverlappingArea(context.other) };
		glm::vec2 reflectionAxis{ overlap.w < overlap.h ? glm::vec2{1, 0} : glm::vec2{0,1} };
		m_Velocity = glm::reflect(m_Velocity, reflectionAxis);
		break;
	}
	default:
		break;
	}

	// Move object back to respect collision

	Owner().GetTransform().TranslatePosition((m_LastMovement.from - m_LastMovement.to));
	std::cout << ((m_LastMovement.from - m_LastMovement.to)).x << ", " << ((m_LastMovement.from - m_LastMovement.to)).y << "\n";
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

} // !eng