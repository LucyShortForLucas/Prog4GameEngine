#pragma once

#include "AbstractComponent.h"
#include "Serialization.h"

#include <ColorMap.h>
#include <Services.h>

namespace tron {

DECL_COMPONENT(TronGameSetup, public eng::AbstractComponent)
public:
//---- Ctor
	TronGameSetup(eng::Actor& owner) :
		eng::AbstractComponent(owner) {};

//---- Gameloop methods
	void Start();

};
REGISTER_COMPONENT(TronGameSetup)

} // !tron