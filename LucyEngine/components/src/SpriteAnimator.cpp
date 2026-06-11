#include "SpriteAnimator.h"
#include "HashedString.h"
#include "Services.h"
#include <variant>

namespace eng {

std::unique_ptr<SpriteAnimator> SpriteAnimator::Deserialize(Actor& owner, const nlohmann::json& json) {
	std::vector<HashedString> frames{};

	for (const std::string& frame : json.value("Frames", nlohmann::ordered_json::array())) {
		frames.emplace_back(HashedString(frame));
	}
	
	if (!frames.empty())
		return std::make_unique<SpriteAnimator>(owner, json.value("SecondsPerFrame", 0.1f), frames);
	return std::make_unique<SpriteAnimator>(owner, json.value("SecondsPerFrame", 0.1f), json.value("FrameSize", glm::vec2{}));
}

nlohmann::ordered_json SpriteAnimator::Serialize() {
	nlohmann::ordered_json j{};

	j["SecondsPerFrame"] = m_SecondsPerFrame;
	j["Frames"] = nlohmann::ordered_json::array();
	j["FrameSize"] = m_FrameSize;

	for (const auto& frame : m_Frames) {
		j["Frames"].emplace_back(frame.text);
	}

	return j;
}

static SDL_FRect FrameToSourceRect(glm::vec2 totalSize, glm::vec2 spriteSize, int& frameNum) {
	const int columns{ static_cast<int>(totalSize.x / spriteSize.x) };
	const int rows{ static_cast<int>(totalSize.y / spriteSize.y) };
	const int totalFrames{ columns * rows };

	frameNum = ((frameNum % totalFrames) + totalFrames) % totalFrames;

	const int frameColumn{ frameNum % columns };
	const int frameRow{ frameNum / columns };

	return { spriteSize.x * frameColumn, spriteSize.y * frameRow, spriteSize.x, spriteSize.y };
}

void SpriteAnimator::NextFrame() {
	if (m_SpriteController != nullptr) { // Sprite controller not null, means we want to use it to control the animation
		if (++m_CurrentFrame == m_Frames.size())
			m_CurrentFrame = 0;
		m_SpriteController->SetSprite(m_Frames.at(m_CurrentFrame));
		return;
	}

	// If sprite controller is null, we assume texture renderer exists.

	m_TextureRenderer->SetSourceRect(FrameToSourceRect(m_TextureRenderer->GetTextureSize(), m_FrameSize, ++m_CurrentFrame));
}

void SpriteAnimator::PreviousFrame() {
	if (m_SpriteController != nullptr) { // Sprite controller not null, means we want to use it to control the animation
		if (--m_CurrentFrame < 0)
			m_CurrentFrame = m_Frames.size()-1;
		m_SpriteController->SetSprite(m_Frames.at(m_CurrentFrame));
		return;
	}

	// If sprite controller is null, we assume texture renderer exists.

	m_TextureRenderer->SetSourceRect(FrameToSourceRect(m_TextureRenderer->GetTextureSize(), m_FrameSize, --m_CurrentFrame));
}

void SpriteAnimator::Start() {
	if (m_FrameSize == glm::vec2{}) { // If frame size = 0, let frames be managed by the sprite controller
		m_SpriteController = Owner().GetComponent<SpriteController>();
		if (m_SpriteController == nullptr)
			service::logger.Get().LogError("SpriteAnimator " + std::to_string((std::uintptr_t)this) + " does not have a SpriteController.");

		m_SpriteController->SetSprite(m_Frames.at(0));
		return;
	}

	// if frame size != 0, assume the user wants to automatically turn this objects texture into a spritemap

	m_TextureRenderer = Owner().GetComponent<TextureRenderer>();
	if (m_TextureRenderer == nullptr)
		service::logger.Get().LogError("SpriteAnimator " + std::to_string((std::uintptr_t)this) + " does not have a TextureRenderer.");

	m_TextureRenderer->SetSourceRect({ 0, 0, m_FrameSize.x, m_FrameSize.y });
}

void SpriteAnimator::Update() {
	if (m_SecondsPerFrame < 0)
		return;
	m_SecondsPassed += service::gameTime.Get().DeltaTime();

	if (m_SecondsPassed < m_SecondsPerFrame)
		return;

	m_SecondsPassed -= m_SecondsPerFrame;

	NextFrame();
}

int SpriteAnimator::GetCurrentFrame() {
	return m_CurrentFrame;
}

}