#include "MovePhysicsBody.h"

#include <Services.h>

namespace tron {

bool MovePhysicsBody::Execute(eng::Actor& actor) {
	if (!m_ActorPhysicsBodies.contains(&actor)) {
		m_ActorPhysicsBodies[&actor] = actor.GetComponent<eng::PhysicsBody>();
		if (m_ActorPhysicsBodies[&actor] == nullptr) {
			eng::service::logger.Get().LogError("Actor " + std::to_string((std::uintptr_t)&actor) + " does not have a PhysicsBody.");
			return false;
		}
	}

	m_ActorPhysicsBodies[&actor]->SetVelocity(m_Velocity);
	return true;
}

}