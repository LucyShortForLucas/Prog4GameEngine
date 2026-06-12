#include "ShootPlayerBullet.h"

#include <Services.h>
#include <SpriteAnimator.h>

namespace tron {

ShootPlayerBullet::~ShootPlayerBullet() {
			// Whenever a MoveTurret command is destroyed, we must assume pointers have potentially gone stale as well. 
			// We could have a complicated system to notify the static data of the destruction of actors, but honestly just
			// forcing the commands to re-cache the non-stale ones whenever command is destroyed is a negligable overhead.
	s_ActorTurrets.clear();
}

bool ShootPlayerBullet::Execute(eng::Actor& actor) {
	if (!s_ActorTurrets.contains(&actor)) {
		s_ActorTurrets[&actor] = actor.GetComponent<tron::Turret>();
		if (s_ActorTurrets[&actor] == nullptr) {
			eng::service::logger.Get().LogError("Actor " + std::to_string((std::uintptr_t)&actor) + " does not have a Turret.");
			return false;
		}
	}

	s_ActorTurrets[&actor]->Shoot();
}

}