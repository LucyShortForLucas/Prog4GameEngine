#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <Services.h>

#include <Actor.h>
#include <AabbCollider.h>

#include <EventSource.h>

namespace tron {

namespace event {

struct PlayerDied {
	eng::Actor& player;
	int livesLeft;
};

}

DECL_COMPONENT(PlayerLives, public eng::AbstractComponent, public eng::AbstractEventListener<eng::event::AabbCollisionEnter>)
	DECL_EVENT(PlayerDied)
public:
//---- Ctor
	PlayerLives(eng::Actor& owner) :
		eng::AbstractComponent(owner) {};

//---- Gameloop

	void OnEnable() override;
	void OnDisable() override;

//---- Event handler
	void OnEvent(const eng::event::AabbCollisionEnter& context);

private:
	int m_Lives{ 3 };
};
REGISTER_COMPONENT(PlayerLives)

} // !tron