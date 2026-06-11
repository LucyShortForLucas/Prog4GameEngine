#include "ResourceLoader.h"
#include <SDL3/SDL.h>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <map>
#include "Utils.h"
#include "Texture2D.h"
#include "Font.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <fstream>
#include <iostream>

namespace eng {

eng::SdlResourceLoader::SdlResourceLoader() {
	if (!TTF_Init()) {
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

eng::SdlResourceLoader::~SdlResourceLoader() {
	m_TextureUptrs.clear();
	m_FontUptrs.clear();
}

dae::Texture2D* const eng::SdlResourceLoader::LoadTexture(const std::string& file) {
	const auto fullPath = std::string(SDL_GetBasePath()) + "Data\\Textures\\" + file;
	const auto filename = std::filesystem::path(fullPath).filename().string();

	if (m_TextureUptrs.find(filename) == m_TextureUptrs.end()) {
		const auto& tex{ m_TextureUptrs.insert(std::pair(filename, std::make_unique<dae::Texture2D>(fullPath))) };
		SDL_SetTextureScaleMode(m_TextureUptrs.at(filename)->GetSDLTexture(), SDL_SCALEMODE_NEAREST);
	}

	return m_TextureUptrs.at(filename).get();
}

dae::Font* const eng::SdlResourceLoader::LoadFont(const std::string& file, uint8_t size) {
	const auto fullPath = std::string(SDL_GetBasePath()) + "Data\\Fonts\\" + file;
	const auto filename = std::filesystem::path(fullPath).filename().string();

	const auto key = std::pair<std::string, uint8_t>(filename, size);

	if (m_FontUptrs.find(key) == m_FontUptrs.end()) m_FontUptrs.insert(std::make_pair(key, std::make_unique<dae::Font>(fullPath, size)));

	return m_FontUptrs.at(key).get();
}

nlohmann::json* const eng::SdlResourceLoader::LoadJson(const std::string& file) {
	const auto fullPath = std::string(SDL_GetBasePath()) + "Data\\Json\\" + file;
	const auto filename = std::filesystem::path(fullPath).filename().string();

	if (m_JsonUptrs.find(filename) == m_JsonUptrs.end()) {
		auto j{ std::make_unique<nlohmann::json>() };
		std::ifstream stream{ fullPath };
		stream >> *j;
		m_JsonUptrs.insert(std::pair(filename, std::move(j)));
	}

	return m_JsonUptrs.at(filename).get();
}

ColorMap* const eng::SdlResourceLoader::LoadColorMap(const std::string& file) {
	const auto fullPath = std::string(SDL_GetBasePath()) + "Data\\Textures\\" + file;
	const auto filename = std::filesystem::path(fullPath).filename().string();

	if (m_JsonUptrs.find(filename) == m_JsonUptrs.end()) {
		//---- Load the image as an SDL_Surface
		SDL_Surface* raw = SDL_LoadPNG(fullPath.c_str());
		if (!raw)
			service::logger.Get().LogError(std::string("SDL_LoadPNG failed: ") + SDL_GetError());

		// Convert to a guaranteed 32-bit RGBA format so pixel math is predictable
		SDL_Surface* surface = SDL_ConvertSurface(raw, SDL_PIXELFORMAT_RGBA8888);
		SDL_DestroySurface(raw);
		if (!surface)
			service::logger.Get().LogError(std::string("SDL_ConvertSurface failed: ") + SDL_GetError());


		//---- Lock before accessing pixels directly
		if (!SDL_LockSurface(surface))
			service::logger.Get().LogError(std::string("SDL_LockSurface failed: ") + SDL_GetError());

		const Uint32* pixels = static_cast<const Uint32*>(surface->pixels);
		const int pitchPixels = surface->pitch / 4;

		std::vector<SDL_Color> colors;
		colors.reserve(surface->w * surface->h);

		for (int y = 0; y < surface->h; ++y) {
			for (int x = 0; x < surface->w; ++x) {
				Uint32 pixel = pixels[y * pitchPixels + x];

				SDL_Color c;
				c.r = (pixel >> 24) & 0xFF;
				c.g = (pixel >> 16) & 0xFF;
				c.b = (pixel >> 8) & 0xFF;
				c.a = (pixel >> 0) & 0xFF;

				colors.emplace_back(c);
			}
		}


		m_ColorMaps.insert(std::pair(filename, std::make_unique<ColorMap>(std::move(colors), surface->w)));

		SDL_UnlockSurface(surface);
		SDL_DestroySurface(surface);
	}
	
	return m_ColorMaps.at(filename).get();

}


}