#pragma once
#include <SDL3/SDL.h>

#include "AbstractComponent.h"
#include "Texture2D.h"
#include "Font.h"
#include "Serialization.h"

#include <string>
#include <memory>

namespace eng {

DECL_COMPONENT(TextRenderer)
public: //--------------- Constructor/Destructor/copy/move --------------
	TextRenderer(Actor& owner, const std::string& text = "", const std::string& fontPath = "Lingua.otf", unsigned int size = 24, SDL_Color color = { 255,255,255,255 });
	~TextRenderer() = default;

public: //------------------ General methods --------------------------

	void SetText(const std::string& text);

public: //------------------ Gameloop methods --------------------------

	void Update() override;
	void Render() override;


private: //---------------------------|Fields|----------------------------
	SDL_Color m_Color{ 255,255,255,255 }; // white
	std::string m_Text;
	std::string m_FontPath;
	unsigned int m_Size;
	bool m_NeedsUpdate{};
	dae::Font* m_FontPtr;
	std::unique_ptr<dae::Texture2D> m_TextTextureUptr{};

}; // !TextRenderer

}