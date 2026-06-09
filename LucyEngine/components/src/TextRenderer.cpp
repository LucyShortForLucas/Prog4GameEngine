#include "TextRenderer.h"
#include <stdexcept>
#include "Font.h"
#include "Texture2D.h"
#include "Transform2d.h"
#include <cstdint>
#include "ResourceLoader.h"
#include <filesystem>
#include <string>
#include "Services.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_oldnames.h>

namespace eng {

eng::TextRenderer::TextRenderer(eng::Actor& owner, const std::string& text, const std::string& fontPath, unsigned int size, SDL_Color color) :
	AbstractComponent(owner),
	m_Color(color),
	m_Text(text),
	m_FontPath(fontPath),
	m_Size(size),
	m_NeedsUpdate(true) {
	m_FontPtr = eng::service::resources.Get().LoadFont(fontPath, static_cast<uint8_t>(m_Size));
}

nlohmann::ordered_json eng::TextRenderer::Serialize() {
	nlohmann::ordered_json j{};

	j["Text"] = m_Text;
	j["FontPath"] = m_FontPath;
	j["Size"] = m_Size;
	j["Color"] = m_Color;

	return j;
}

std::unique_ptr<TextRenderer> TextRenderer::Deserialize(Actor& owner, const nlohmann::json& json) {
	return std::make_unique<TextRenderer>(owner,
		json.value("Text", ""),
		json.value("FontPath", ""),
		json.value("Size", 12u),
		json.value("Color", SDL_Color{})
	);
}

void eng::TextRenderer::SetText(const std::string& text) {
	m_Text = text;
	m_NeedsUpdate = true;
}

void eng::TextRenderer::Start() {
	m_TextureRendererPtr = Owner().GetComponent<TextureRenderer>();
	if (m_TextureRendererPtr == nullptr)
		service::logger.Get().LogError("TextRenderer " + std::to_string((std::uintptr_t)this) + " does not have a TextureRenderer.");
}

void eng::TextRenderer::Update() {
	if (m_NeedsUpdate) {
		const auto f_Surf = TTF_RenderText_Blended(m_FontPtr->GetFont(), m_Text.c_str(), m_Text.size(), m_Color);

		if (f_Surf == nullptr) {
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(eng::service::renderer.Get().GetSDLRenderer(), f_Surf);

		if (texture == nullptr) {
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_DestroySurface(f_Surf);
		m_TextTextureUptr = std::make_unique<dae::Texture2D>(texture);
		m_TextureRendererPtr->SetTexture(m_TextTextureUptr.get());
		m_NeedsUpdate = false;
	}
}

}