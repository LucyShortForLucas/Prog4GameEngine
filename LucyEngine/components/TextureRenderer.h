#pragma once

#pragma once

#include "AbstractComponent.h"
#include "Texture2D.h"
#include <SDL3/SDL.h>

namespace eng::cpt {

class TextureRenderer final : public eng::AbstractComponent {
public: //--------------- Constructor/Destructor/copy/move --------------

	TextureRenderer(eng::Actor& owner, const std::string& texturePath, glm::ivec2 size = { -1, -1 }, SDL_Rect sourceRect = { -1, -1, -1, -1 }, unsigned int layer = 0);
	~TextureRenderer() override = default;

	TextureRenderer(const TextureRenderer& other) = delete;
	TextureRenderer& operator=	(const TextureRenderer&) = delete;

	TextureRenderer(TextureRenderer&&) = delete;
	TextureRenderer& operator=	(TextureRenderer&&) = delete;

public: //------------------------ Texture Methods -------------------------

	void LoadTexture(const std::string& file);
	void SetSourceRect(SDL_Rect rect);

	static float s_GlobalScale;

public: //------------------------ Gameloop Methods -------------------------

	void Render() override;

private: //--------------------------- Fields ----------------------------
	unsigned int m_Layer{};
	std::string m_TexturePath;
	dae::Texture2D* m_TexturePtr{};
	glm::ivec2 m_Size{};
	SDL_Rect m_SourceRect{};

}; // !TextureRenderer

} // !cpt