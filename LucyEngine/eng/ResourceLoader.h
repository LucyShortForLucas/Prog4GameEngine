#pragma once

#include <map>

#include "Texture2D.h"
#include "Font.h"
#include "Services.h"
#include "Utils.h"

namespace eng {

class SdlResourceLoader final : public service::IResourceLoader {
public:
	SdlResourceLoader();
	~SdlResourceLoader() override;

	dae::Texture2D* LoadTexture(const std::string& file) override;
	dae::Font* LoadFont(const std::string& file, uint8_t size) override;
private:
	std::map<std::string, std::unique_ptr<dae::Texture2D>> m_TextureUptrs;
	std::map<std::pair<std::string, uint8_t>, std::unique_ptr<dae::Font>> m_FontUptrs;
};

}