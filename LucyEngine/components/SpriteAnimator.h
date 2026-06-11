#pragma once

#include "Serialization.h"
#include "SpriteController.h"

namespace eng {

/// @brief	A
DECL_COMPONENT(SpriteAnimator, public AbstractComponent)
public:
	//---- Ctor
																					/// Create an animator that uses a Sprite Controller to define its frames
	SpriteAnimator(Actor& owner, const float secondsPerFrame, std::vector<HashedString> frames)
		: AbstractComponent(owner), m_SecondsPerFrame(secondsPerFrame), m_Frames(frames) {};
																					/// Create an animator that automatically turns its texture into frames of animation
	SpriteAnimator(Actor& owner, const float secondsPerFrame, glm::vec2 frameSize)
		: AbstractComponent(owner), m_SecondsPerFrame(secondsPerFrame), m_FrameSize(frameSize) {};

	//---- Serialization
	nlohmann::ordered_json Serialize() override;

	//---- Animation methods
	void NextFrame();
	void PreviousFrame();
	int GetCurrentFrame();

	//---- Gameloop methods
	void Start() override;
	void Update() override;

private:
	SpriteController* m_SpriteController{};
	TextureRenderer* m_TextureRenderer{};
	std::vector<HashedString> m_Frames{};
	const float m_SecondsPerFrame{-1};
	float m_SecondsPassed{};
	int m_CurrentFrame{};
	glm::vec2 m_FrameSize{};
};
REGISTER_COMPONENT(SpriteAnimator)
} // !eng