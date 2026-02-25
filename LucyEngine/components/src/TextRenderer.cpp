#include "TextRenderer.h"
#include <stdexcept>
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"
#include <cstdint>
#include "ResourceLoader.h"
#include <filesystem>
#include <string>
#include "Services.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_oldnames.h>


eng::TextRenderer::TextRenderer(eng::Actor& owner, const std::string& text, const std::string& fontPath, unsigned int size, SDL_Color color) :
	AbstractComponent(owner),
	m_Text(text), m_NeedsUpdate(true),
	m_Size(size),
	m_FontPath(fontPath),
	m_Color(color) {
	m_FontPtr = eng::service::resources.Get().LoadFont(fontPath, static_cast<uint8_t>(size));
}

void eng::TextRenderer::SetText(const std::string& text) {
	m_Text = text;
	m_NeedsUpdate = true;
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
		m_NeedsUpdate = false;
	}
}

void eng::TextRenderer::Render() {
	if (m_TextTextureUptr == nullptr) return;

	const glm::vec2 f_Pos{ Owner().GetTransform().GetLocal().position };
	const auto f_Size{ m_TextTextureUptr->GetSize() };
	eng::service::renderer.Get().RenderTexture({ *m_TextTextureUptr, SDL_FRect{ f_Pos.x, f_Pos.y, f_Size.x, f_Size.y }, SDL_FRect{ 0, 0, f_Size.x, f_Size.y } });

}