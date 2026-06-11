#include "MoveTurret.h"

#include <Services.h>
#include <SpriteAnimator.h>

namespace tron {

MoveTurret::MoveTurret(bool clockwise, float minSecDelay) :
	m_Clockwise{ clockwise }, m_MinSecDelay{ minSecDelay } {};

MoveTurret::~MoveTurret() {
			// Whenever a MoveTurret command is destroyed, we must assume pointers have potentially gone stale as well. 
			// We could have a complicated system to notify the static data of the destruction of actors, but honestly just
			// forcing the commands to re-cache the non-stale ones whenever command is destroyed is a negligable overhead.
			s_ActorAnimators.clear();
		}

bool MoveTurret::Execute(eng::Actor& actor) {
			if (!s_ActorAnimators.contains(&actor)) {
				s_ActorAnimators[&actor] = actor.GetComponent<eng::SpriteAnimator>();
				if (s_ActorAnimators[&actor] == nullptr) {
					eng::service::logger.Get().LogError("Actor " + std::to_string((std::uintptr_t)&actor) + " does not have a SpriteAnimator.");
					return false;
				}
			}

			m_SecondsPassed += eng::service::gameTime.Get().DeltaTime();
			if (m_SecondsPassed < m_MinSecDelay)
				return false;

			m_SecondsPassed = 0;

			if (!m_Clockwise)
				s_ActorAnimators[&actor]->NextFrame();
			else
				s_ActorAnimators[&actor]->PreviousFrame();

			int row{ (s_ActorAnimators[&actor]->GetCurrentFrame()/9)%4};

			switch (row) {
			case 0:
				actor.GetTransform().SetLocalPosition(16,-16);
				break;
			case 1:
				actor.GetTransform().SetLocalPosition(-16, -16);
				break;
			case 2:
				actor.GetTransform().SetLocalPosition(-16, 16);
				break;
			case 3:
				actor.GetTransform().SetLocalPosition(16, 16);
				break;
			}

			return true;
		}

}