#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"

namespace eng {

class FpsTracker final : public AbstractComponent {
public: //--------------- Constructor/Destructor/copy/move --------------

	FpsTracker(Actor& owner) : AbstractComponent(owner) {};
	~FpsTracker() = default;

	FpsTracker(const FpsTracker&) = delete;
	FpsTracker& operator=	(const FpsTracker&) = delete;

	FpsTracker(FpsTracker&&) = delete;
	FpsTracker& operator=	(FpsTracker&&) = delete;

public: //------------------ Gameloop Methods --------------------------
	void OnEnable() override;
	void OnDisable() override;
	void Update() override;

private: //--------------------------- Component references ----------------------------

	TextRenderer* m_TextRendererPtr{};

}; // !FpsTracker

}