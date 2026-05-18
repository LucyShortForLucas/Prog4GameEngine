#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

namespace eng {

DECL_COMPONENT(Rotator)
public: //--------------- Constructor/Destructor/copy/move --------------

	Rotator(Actor& owner) : AbstractComponent(owner) {};
	~Rotator() = default;

public: //------------------ Gameloop Methods --------------------------
	void Update() override;

}; // !Rotator

}