#pragma once

#include "AbstractComponent.h"
#include "PathfindGraph.h"
#include "Serialization.h"

#include <EventSource.h>
#include <AabbCollider.h>

namespace tron {

DECL_COMPONENT(EnemyBulletCollisions, public eng::AbstractComponent, public eng::AbstractEventListener<eng::event::AabbCollisionEnter>)
public:
	//---- Ctor
	EnemyBulletCollisions(eng::Actor& owner) : eng::AbstractComponent(owner){};

//---- Gameloop

	void OnEnable() override;
	void OnDisable() override;

//---- Event handlers
	void OnEvent(const eng::event::AabbCollisionEnter& context);
		
}; // !FpsTracker
REGISTER_COMPONENT(EnemyBulletCollisions)

} // !tron