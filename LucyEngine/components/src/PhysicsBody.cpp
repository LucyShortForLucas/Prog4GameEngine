#include "PhysicsBody.h"
#include "Actor.h"
#include "AabbCollider.h"
#include "Services.h"

namespace eng {

std::unique_ptr<PhysicsBody> PhysicsBody::Deserialize(Actor& owner, const nlohmann::json& json) {
	auto result{ std::make_unique<PhysicsBody>(owner) };
	result->m_Velocity = json.value("Velocity", glm::vec2{});
	return result;
}

void PhysicsBody::OnEnable() {
	auto collider { Owner().GetComponent<AabbCollider>() };
	collider->SubscribeAabbCollisionEnter(*this);
}

void PhysicsBody::OnDisable() {
	auto collider{ Owner().GetComponent<AabbCollider>() };
	collider->UnsubscribeAabbCollisionEnter(*this);
}

void PhysicsBody::FixedUpdate() {
	if (m_Velocity == glm::vec2{})
		return;

	auto& transform{ Owner().GetTransform() };

	m_LastMovement.from = transform.GetGlobal().position;
	transform.TranslatePosition(m_Velocity * service::gameTime.Get().FixedDeltaTime());
	m_LastMovement.to = transform.GetGlobal().position;
}



void PhysicsBody::OnEvent(const event::AabbCollisionEnter& context) {
	if (!context.other.Owner().GetComponent<PhysicsBody>())
		return; // No physics body in collison -> don't care

	float moveBackAmount{ LineRectIntersect(m_LastMovement, context.other.GetRect()) };

	Owner().GetTransform().TranslatePosition((m_LastMovement.from - m_LastMovement.to) * moveBackAmount);
}


} // !eng