#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

namespace tron {

DECL_COMPONENT(GameSceneSelector, public eng::AbstractComponent)
public:
	//---- Ctor

	GameSceneSelector(eng::Actor& owner) : eng::AbstractComponent(owner) {};

//---- Gameloop Methods 
	void Start() override;
	void Update() override;

private:

	eng::TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker
REGISTER_COMPONENT(GameSceneSelector)

} // !tron