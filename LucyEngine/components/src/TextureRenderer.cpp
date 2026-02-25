#include "TextureRenderer.h"
#include "Actor.h"
#include "Transform.h"
#include "Services.h"

#include <filesystem>

namespace eng {
float TextureRenderer::s_GlobalScale{ 1.f };

void TextureRenderer::LoadTexture(const std::string& file) {
	m_TexturePtr = eng::service::resources.Get().LoadTexture(file);
	m_TexturePath = file;
}
void TextureRenderer::SetSourceRect(SDL_FRect rect) {
	m_SourceRect = rect;
}


TextureRenderer::TextureRenderer(eng::Actor& owner, const std::string& texturePath, glm::ivec2 size, SDL_FRect sourceRect, unsigned int layer) : AbstractComponent(owner), m_Layer(layer), m_TexturePath(texturePath),  m_Size(size), m_SourceRect(sourceRect) {
	LoadTexture(texturePath);
	auto f_TextureSize{ m_TexturePtr->GetSize() };
	if (m_Size.x == -1)  m_Size = f_TextureSize;
	if (m_SourceRect.x == -1) m_SourceRect = { 0,0, f_TextureSize.x, f_TextureSize.y };
}
void TextureRenderer::Render() {
	auto f_Pos = Owner().GetTransform().GetGlobal().position;
	f_Pos.x *= s_GlobalScale;
	f_Pos.y *= s_GlobalScale;

	TextureMapping m_Mapping{ *m_TexturePtr, SDL_FRect{static_cast<float>(f_Pos.x), static_cast<float>(f_Pos.y),static_cast<float>(m_Size.x * s_GlobalScale), static_cast<float>(m_Size.y * s_GlobalScale)}, m_SourceRect };

	if (m_Layer == 0) eng::service::renderer.Get().RenderTexture(m_Mapping);
	else eng::service::renderer.Get().DelayRender(m_Layer, m_Mapping);
}

} // !cpt