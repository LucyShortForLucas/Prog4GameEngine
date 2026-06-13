#pragma once

#include <bitset>
#include <glm/glm.hpp>

#include <SceneTree.h>

namespace tron {

class PathfindGraph final {
public:
//---- Ctor
	PathfindGraph(const std::string& level);

//---- Level Layout API
																					/// Returns true if the 2x2 tile area with [x, y] in the top-left is free
	bool Test2x2Free(glm::vec2 pos) const;											/// Returns true if the 2x2 tile area is free AND not occupied by an actor tagged as "Tank" 
	bool Test2x2Occupied(glm::vec2 pos, const eng::SceneTree& tree) const;
	glm::vec2 Tile2x2ToPosition(glm::vec2 pos) const;
	glm::vec2 PositionToTile2x2(glm::vec2 pos) const;
	glm::vec2 GetRandomUnoccupied2x2(const eng::SceneTree& tree) const;

//---- Pathfinding API
	std::vector<glm::vec2> FindPath(glm::vec2 from, glm::vec2 to);

private:
	const std::bitset<29 * 27> m_LevelLayout2x2; // 2d bitset where each bit represents a 2x2 section of the level, and wether it is accessible or blocked
};

} // !tron