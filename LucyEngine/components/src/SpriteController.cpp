#include "SpriteController.h"
#include "Actor.h"
#include "Services.h"

namespace eng {

std::unique_ptr<SpriteController> SpriteController::Deserialize(Actor& owner, const nlohmann::json& json) {
	std::map<HashedString, std::pair<std::string, glm::vec2>> sprites{};

	for (auto& [key, jSprite] : json["Sprites"].items()) {
		std::string texture = jSprite["Texture"].get<std::string>();
		glm::vec2 position = jSprite["Position"].get<glm::vec2>();

		sprites[HashedString(key)] = { texture, position };
	}

	return std::make_unique<SpriteController>(owner, sprites, json.value("Size", glm::vec2{}));
}

nlohmann::ordered_json SpriteController::Serialize() {
	nlohmann::ordered_json j{};

	j["Size"] = m_Size;

	for (auto& sprite : m_Sprites) {
		nlohmann::ordered_json jSprite{};
		jSprite["Texture"] = sprite.second.first;
		jSprite["Position"] = sprite.second.second;
		j["Sprites"][sprite.first.text] = jSprite;
	}

	return j;
}

void SpriteController::Start() {
	m_TextureRenderer = Owner().GetComponent<TextureRenderer>();
	if (m_TextureRenderer == nullptr)
		service::logger.Get().LogError("Physicsbody " + std::to_string((std::uintptr_t)this) + " does not have a collider.");
}

void SpriteController::SetSprite(HashedString sprite) {
	auto& pair{ m_Sprites.at(sprite) };
	m_TextureRenderer->LoadTexture(pair.first);
	m_TextureRenderer->SetSourceRect({ pair.second.x, pair.second.y, m_Size.x, m_Size.y});
}

}