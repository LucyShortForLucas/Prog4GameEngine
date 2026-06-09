#include "TextureRenderer.h"
#include "Actor.h"
#include "Transform2d.h"
#include "Services.h"

#include <filesystem>

namespace eng {

void TextureRenderer::LoadTexture(const std::string& file) {
	if (file != "")
		m_TexturePtr = eng::service::resources.Get().LoadTexture(file);
	m_TexturePath = file;
}
void TextureRenderer::SetSourceRect(SDL_FRect rect) {
	m_SourceRect = rect;
}

void TextureRenderer::SetTexture(dae::Texture2D* texture, glm::ivec2 size, SDL_FRect sourceRect, unsigned int layer) {
	m_TexturePtr = texture;
	auto f_TextureSize{ m_TexturePtr->GetSize() };

	m_Size = size;
	m_SourceRect = sourceRect;

	if (m_Size.x == -1)  m_Size = f_TextureSize;
	if (m_SourceRect.x == -1) m_SourceRect = { 0,0, f_TextureSize.x, f_TextureSize.y };
}

nlohmann::ordered_json TextureRenderer::Serialize() {
	nlohmann::ordered_json j{};

	j["TexturePath"] = m_TexturePath;
	j["Size"] = m_Size;
	j["SourceRect"] = m_SourceRect;
	j["Layer"] = m_Layer;

	return j;
}

std::unique_ptr<TextureRenderer> TextureRenderer::Deserialize(Actor& owner, const nlohmann::json& json) {
	return std::make_unique<TextureRenderer>(owner,
		json.value("TexturePath", ""),
		json.value("Size", glm::ivec2{-1, -1}),
		json.value("SourceRect", SDL_FRect{-1, -1, -1, -1}),
		json.value("Layer", 0u)
	);
}

TextureRenderer::TextureRenderer(eng::Actor& owner, const std::string& texturePath, glm::ivec2 size, SDL_FRect sourceRect, unsigned int layer) 
	: AbstractComponent(owner), m_Layer(layer), m_TexturePath(texturePath),  m_Size(size), m_SourceRect(sourceRect) {
	LoadTexture(texturePath);
	if (m_TexturePtr == nullptr)
		return;
	auto f_TextureSize{ m_TexturePtr->GetSize() };
	if (m_Size.x == -1)  m_Size = f_TextureSize;
	if (m_SourceRect.x == -1) m_SourceRect = { 0,0, f_TextureSize.x, f_TextureSize.y };
}

void TextureRenderer::Render() {
	auto f_Pos = Owner().GetTransform().GetGlobal().position;

	TextureMapping m_Mapping{ *m_TexturePtr, SDL_FRect{static_cast<float>(f_Pos.x), static_cast<float>(f_Pos.y),static_cast<float>(m_Size.x), static_cast<float>(m_Size.y)}, m_SourceRect };

	if (m_Layer == 0) eng::service::renderer.Get().RenderTexture(m_Mapping);
	else eng::service::renderer.Get().DelayRender(m_Layer, m_Mapping);
}

} // !cpt