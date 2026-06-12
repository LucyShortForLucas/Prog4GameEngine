#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <Services.h>
#include <PhysicsBody.h>

namespace tron {


DECL_COMPONENT(PlayerBulletBounce, public eng::AbstractComponent, eng::AbstractEventListener<eng::event::PhysicsBodyBounced>)
public:
//---- Ctor
PlayerBulletBounce(eng::Actor& owner) :
		eng::AbstractComponent(owner) {};

//---- Gameloop methods
	void OnEnable() override;
	void OnDisable() override;

//---- Event handlers
	void OnEvent(const eng::event::PhysicsBodyBounced& context);

private:
	int m_Bounces{};
};
REGISTER_COMPONENT(PlayerBulletBounce)

} // !tron