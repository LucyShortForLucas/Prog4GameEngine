#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "Serialization.h"

namespace eng {

DECL_COMPONENT(FpsTracker)
public: //--------------- Constructor/Destructor/copy/move --------------

	FpsTracker(Actor& owner) : AbstractComponent(owner) {};
	~FpsTracker() = default;

//------------------ Gameloop Methods --------------------------
	void OnEnable() override;
	void OnDisable() override;
	void Update() override;

private: //--------------------------- Component references ----------------------------

	TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker

}