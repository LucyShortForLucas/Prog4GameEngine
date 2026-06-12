#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

namespace tron {

DECL_COMPONENT(FpsTracker, public eng::AbstractComponent)
public:
	//---- Ctor

	FpsTracker(eng::Actor& owner) : eng::AbstractComponent(owner) {};

//---- Gameloop Methods 
	void OnEnable() override;
	void OnDisable() override;
	void Update() override;

private:

	eng::TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker
REGISTER_COMPONENT(FpsTracker)

} // !tron