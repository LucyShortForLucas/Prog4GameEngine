#include "Pathfinding.h"

#include <Services.h>

namespace tron {

std::unique_ptr<Pathfinding> Pathfinding::Deserialize(eng::Actor& owner, const nlohmann::json& json) {
	return std::make_unique<Pathfinding>(owner, json.value("LevelPath", ""));
}

PathfindGraph& Pathfinding::GetGraph() {
	return m_PathindGraph;
}

void Pathfinding::Render() {
#ifndef  NDEBUG
	auto& renderer{ *eng::service::renderer.Get().GetSDLRenderer() };
	SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);

	for (int y{}; y < 27; ++y) {
		for (int x{}; x < 29; ++x) {
			if (!m_PathindGraph.Test2x2Free({ x, y }))
				continue;
			auto pos{ m_PathindGraph.Tile2x2ToPosition({x, y}) };
			SDL_FRect rect{ pos.x - 3, pos.y - 3, 6, 6 };
			SDL_RenderRect(&renderer, &rect);
		}
	}
#endif // ! NDEBUG

}

} // !tron