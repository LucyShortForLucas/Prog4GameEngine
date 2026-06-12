#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

namespace tron {

DECL_COMPONENT(RepoolAfterNSeconds, public eng::AbstractComponent)
public: 
	//---- Ctor

	RepoolAfterNSeconds(eng::Actor& owner, float seconds, unsigned int tag) : eng::AbstractComponent(owner), m_TimeToLive{ seconds }, m_Tag{tag} {};

//---- Gameloop Methods 
	void Update() override;

private:
	const float m_TimeToLive{};
	float m_TimeLived{};
	unsigned int m_Tag{};

}; // !FpsTracker
REGISTER_COMPONENT(RepoolAfterNSeconds)

} // !tron