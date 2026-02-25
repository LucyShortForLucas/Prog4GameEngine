#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"

namespace eng {

class Rotator : public AbstractComponent {
public: //--------------- Constructor/Destructor/copy/move --------------

	Rotator(Actor& owner) : AbstractComponent(owner) {};
	~Rotator() = default;

	Rotator(const Rotator&) = delete;
	Rotator& operator=	(const Rotator&) = delete;

	Rotator(Rotator&&) = delete;
	Rotator& operator=	(Rotator&&) = delete;

public: //------------------ Gameloop Methods --------------------------
	void Update() override;

}; // !Rotator

}