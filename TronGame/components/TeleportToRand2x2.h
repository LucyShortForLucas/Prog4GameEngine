#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <ColorMap.h>
#include <Services.h>
#include <EventSource.h>
#include <AabbCollider.h>

#include <Pathfinding.h>

namespace tron {

DECL_COMPONENT(TeleportToRand2x2, public eng::AbstractComponent, public eng::AbstractEventListener<eng::event::AabbCollisionEnter>)
public:
//---- Ctor
	TeleportToRand2x2(eng::Actor& owner) :
		eng::AbstractComponent(owner) {};

//---- Gameloop methods

	void Start() override;
	void OnEnable() override;
	void OnDisable() override;

//---- Event handler
	void OnEvent(const eng::event::AabbCollisionEnter& context) override;

private:
	Pathfinding* m_Pathfinding;
};
REGISTER_COMPONENT(TeleportToRand2x2)

} // !tron