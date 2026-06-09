#pragma once

#include "Serialization.h"
#include "SpriteController.h"

namespace eng {

DECL_COMPONENT(SpriteAnimator, public AbstractComponent)
public:
	//---- Ctor
	SpriteAnimator(Actor& owner, const float secondsPerFrame, std::vector<HashedString> frames)
		: AbstractComponent(owner), m_SecondsPerFrame(secondsPerFrame), m_Frames(frames) {};

	//---- Serialization
	nlohmann::ordered_json Serialize() override;

	//---- Gameloop methods
	void Start() override;
	void Update() override;

private:
	SpriteController* m_SpriteController{};
	std::vector<HashedString> m_Frames{};
	const float m_SecondsPerFrame{};
	float m_SecondsPassed{};
	int m_CurrentFrame{};
};

} // !eng