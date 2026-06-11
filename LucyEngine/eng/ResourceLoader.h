#pragma once

#include <map>

#include "Texture2D.h"
#include "Font.h"
#include "Services.h"
#include "Utils.h"
#include "ColorMap.h"

namespace eng {

class SdlResourceLoader final : public service::IResourceLoader {
public:
	SdlResourceLoader();
	~SdlResourceLoader() override;

	dae::Texture2D* const LoadTexture(const std::string& file) override;
	dae::Font* const LoadFont(const std::string& file, uint8_t size) override;
	nlohmann::json* const LoadJson(const std::string& file) override;
	ColorMap* const LoadColorMap(const std::string& file) override;
private:
	std::map<std::string, std::unique_ptr<dae::Texture2D>> m_TextureUptrs;
	std::map<std::pair<std::string, uint8_t>, std::unique_ptr<dae::Font>> m_FontUptrs;
	std::map<std::string, std::unique_ptr<nlohmann::json>> m_JsonUptrs;
	std::map<std::string, std::unique_ptr<ColorMap>> m_ColorMaps;
};

}