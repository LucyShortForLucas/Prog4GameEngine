#pragma once

#include "AbstractComponent.h"
#include "PathfindGraph.h"
#include "Serialization.h"

namespace tron {

DECL_COMPONENT(Pathfinding, public eng::AbstractComponent)
public:
	//---- Ctor
	Pathfinding(eng::Actor& owner, const std::string& level) : eng::AbstractComponent(owner), m_PathindGraph(level) {};

//---- Pathfinding methods 

	PathfindGraph& GetGraph();

	void Render() override;

private:

	PathfindGraph m_PathindGraph;

}; // !FpsTracker
REGISTER_COMPONENT(Pathfinding)

} // !tron