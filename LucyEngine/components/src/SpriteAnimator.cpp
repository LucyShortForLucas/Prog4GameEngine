#include "SpriteAnimator.h"
#include "HashedString.h"
#include "Services.h"

namespace eng {

std::unique_ptr<SpriteAnimator> SpriteAnimator::Deserialize(Actor& owner, const nlohmann::json& json) {
	std::vector<HashedString> frames{};

	for (const std::string& frame : json.value("Frames", nlohmann::ordered_json::array())) {
		frames.emplace_back(HashedString(frame));
	}

	return std::make_unique<SpriteAnimator>(owner, json.value("SecondsPerFrame", 0.1f), frames);
}

nlohmann::ordered_json SpriteAnimator::Serialize() {
	nlohmann::ordered_json j{};

	j["SecondsPerFrame"] = m_SecondsPerFrame;
	j["Frames"] = nlohmann::ordered_json::array();

	for (const auto& frame : m_Frames) {
		j["Frames"].emplace_back(frame.text);
	}

	return j;
}

void SpriteAnimator::Start() {
	m_SpriteController = Owner().GetComponent<SpriteController>();
	if (m_SpriteController == nullptr)
		service::logger.Get().LogError("SpriteAnimator " + std::to_string((std::uintptr_t)this) + " does not have a SpriteController.");

	m_SpriteController->SetSprite(m_Frames.at(0));
}

void SpriteAnimator::Update() {
	m_SecondsPassed += service::gameTime.Get().DeltaTime();

	if (m_SecondsPassed < m_SecondsPerFrame)
		return;

	m_SecondsPassed -= m_SecondsPerFrame;

	if (++m_CurrentFrame >= m_Frames.size())
		m_CurrentFrame = 0;

	m_SpriteController->SetSprite(m_Frames.at(m_CurrentFrame));
}

}