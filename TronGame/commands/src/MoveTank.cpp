#include "MoveTank.h"

#include <Services.h>
#include <SpriteController.h>

namespace tron {

MoveTank::MoveTank(glm::vec2 velocity) :
	m_Velocity{ velocity },
	m_Sprite{ [velocity] {
			if (velocity.x > 0)
				return eng::HashedString{ "Right" };
			if (velocity.x < 0)
				return eng::HashedString{ "Left" };
			if (velocity.y > 0)
				return eng::HashedString{ "Down" };
			if (velocity.y < 0)
				return eng::HashedString{ "Up" };
		}() }  {
};

MoveTank::~MoveTank() {
	// Whenever a MoveTank command is destroyed, we must assume pointers have potentially gone stale as well. 
	// We could have a complicated system to notify the static data of the destruction of actors, but honestly just
	// forcing the commands to re-cache the non-stale ones whenever command is destroyed is a negligable overhead.
	s_ActorPhysicsBodies.clear();
	s_ActorSpriteControllers.clear();
}

bool MoveTank::Execute(eng::Actor& actor) {
	if (!s_ActorPhysicsBodies.contains(&actor)) {
		s_ActorPhysicsBodies[&actor] = actor.GetComponent<eng::PhysicsBody>();
		if (s_ActorPhysicsBodies[&actor] == nullptr) {
			eng::service::logger.Get().LogError("Actor " + std::to_string((std::uintptr_t)&actor) + " does not have a PhysicsBody.");
			return false;
		}
	}

	if (!s_ActorSpriteControllers.contains(&actor)) {
		s_ActorSpriteControllers[&actor] = actor.GetComponent<eng::SpriteController>();
		if (s_ActorSpriteControllers[&actor] == nullptr) {
			eng::service::logger.Get().LogError("Actor " + std::to_string((std::uintptr_t)&actor) + " does not have a SpriteController.");
			return false;
		}
	}

	s_ActorPhysicsBodies[&actor]->SetVelocity(m_Velocity);
	s_ActorSpriteControllers[&actor]->SetSprite(m_Sprite);
	return true;
}

}