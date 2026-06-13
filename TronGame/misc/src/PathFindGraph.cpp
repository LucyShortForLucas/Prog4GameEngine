#include "PathfindGraph.h"

#include <services.h>
#include <ColorMap.h>
#include <random>
#include <unordered_set>

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
	return { static_cast<int>(pos.x-4) / 32, static_cast<int>(pos.y- 134) / 32 };
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

std::vector<glm::vec2> PathfindGraph::FindPath(glm::vec2 from, glm::vec2 to) {
	constexpr int width{ 29 };
	constexpr int height{ 27 };

	auto toIndex = [&](glm::vec2 p) {
		return static_cast<int>(p.x) + static_cast<int>(p.y) * width;
		};

	auto isValid = [&](glm::vec2 p) {
		return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height
			&& Test2x2Free(p);
		};

	if (!isValid(from) || !isValid(to))
		return {};

	auto heuristic = [](glm::vec2 a, glm::vec2 b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
		};

	struct Node {
		glm::vec2 pos;
		float priority;
	};

	struct NodeCompare {
		bool operator()(const Node& a, const Node& b) const {
			return a.priority > b.priority;
		}
	};

	std::priority_queue<Node, std::vector<Node>, NodeCompare> openSet;
	std::unordered_set<int> visited;
	std::unordered_map<int, glm::vec2> cameFrom;

	openSet.push({ from, heuristic(from, to) });
	visited.insert(toIndex(from));

	const glm::vec2 directions[4]{
		{  1,  0 }, { -1,  0 }, {  0,  1 }, {  0, -1 }
	};

	bool found{ false };

	while (!openSet.empty()) {
		Node current{ openSet.top() };
		openSet.pop();

		if (current.pos == to) {
			found = true;
			break;
		}

		for (const auto& dir : directions) {
			glm::vec2 next{ current.pos + dir };

			if (!isValid(next) || visited.count(toIndex(next)))
				continue;

			visited.insert(toIndex(next));
			cameFrom[toIndex(next)] = current.pos;
			openSet.push({ next, heuristic(next, to) });
		}
	}

	if (!found)
		return {};

	std::vector<glm::vec2> positions;
	glm::vec2 current{ to };
	while (!(current == from)) {
		positions.emplace_back(current);
		current = cameFrom[toIndex(current)];
	}
	positions.emplace_back(from);
	std::reverse(positions.begin(), positions.end());

	std::vector<glm::vec2> directionsResult;
	for (size_t i{}; i + 1 < positions.size(); ++i) {
		directionsResult.emplace_back(positions[i + 1] - positions[i]);
	}

	return directionsResult;
}

}