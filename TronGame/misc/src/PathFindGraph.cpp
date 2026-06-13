#include "PathfindGraph.h"

#include <services.h>
#include <ColorMap.h>
#include <random>

namespace tron {

PathfindGraph::PathfindGraph(const std::string& level) :
	m_LevelLayout2x2{ [&level] {
		auto levelLayout{ eng::BlackWhiteMap<30, 28>{ *eng::service::resources.Get().LoadColorMap(level) } };

		std::bitset<29 * 27> result{};

		for (int y{}; y < levelLayout.ColumnSize()-1; ++y) {
			for (int x{}; x < levelLayout.RowSize()-1; x++) {

				if (!levelLayout.At(x, y) && !levelLayout.At(x + 1, y) && !levelLayout.At(x, y + 1) && !levelLayout.At(x + 1, y + 1)) 
					result.set(x + y * 29);

			}
		}

		return result;
	}() } {
}


bool PathfindGraph::Test2x2Free(glm::vec2 pos) const {
	if (pos.x + pos.y * 29 >= m_LevelLayout2x2.size())
		return false;
	return m_LevelLayout2x2.test(pos.x + pos.y * 29);
}

bool PathfindGraph::Test2x2Occupied(glm::vec2 pos, const eng::SceneTree& tree) const {
	if (!Test2x2Free(pos))
		return true;

	for (auto& tank : tree.GetAllActorsWithTag(eng::make_sdbm_hash("Tank"))) {
		if (PositionToTile2x2(tank->GetTransform().GetGlobal().position) == pos)
			return true;
	}

	return false;
}

glm::vec2 PathfindGraph::Tile2x2ToPosition(glm::vec2 pos) const {
	return {(pos.x+1)*32.f+4, (pos.y+1)*32.f+134};
}

glm::vec2 PathfindGraph::PositionToTile2x2(glm::vec2 pos) const {
	return { (pos.x-4) / 32.f, (pos.y- 134) / 32.f };
}

glm::vec2 PathfindGraph::GetRandomUnoccupied2x2(const eng::SceneTree& tree) const {
	static std::random_device rd;  // a seed source for the random number engine
	static std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	static std::uniform_int_distribution<> distrib(0, 29*27);

	glm::vec2 result{};

	do {
		int rand{ distrib(gen) };
		result.x = rand % 29;
		result.y = rand / 27;
	} while (Test2x2Occupied(result, tree));

	return result;
}


}