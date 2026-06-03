#pragma once

#include "Serialization.h"
#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include "EventSource.h"
#include "Transform2d.h"


namespace eng {

class AabbCollider;

namespace event {
struct AabbCollisionEnter {
	AabbCollider& sender;
	AabbCollider& other;
};
struct AabbCollision {
	AabbCollider& sender;
	AabbCollider& other;
};
struct AabbCollisionExit {
	AabbCollider& sender;
	AabbCollider& other;
};
}

DECL_COMPONENT(AabbCollider, public AbstractComponent, public AbstractEventListener<event::GlobalPositionChanged>)
	DECL_EVENT(AabbCollisionEnter)
	DECL_EVENT(AabbCollision)
	DECL_EVENT(AabbCollisionExit)
public: 
//---- Constructor/Destructor

	AabbCollider(Actor& owner, SDL_FRect bounds, int layer = 0, uint32_t layerMask = UINT32_MAX);
	~AabbCollider() = default;

	nlohmann::ordered_json Serialize() override;

//---- Collider methods

	bool OverlapsWith(const AabbCollider& other) const;
	bool OverlapsWith(const SDL_FRect& rect) const;

	SDL_FRect OverlappingArea(const AabbCollider& other) const;
	SDL_FRect OverlappingArea(const SDL_FRect& rect) const;

	SDL_FRect GetBounds() const;
	SDL_FRect GetRect() const;

//---- Gameloop Methods

	void OnEnable() override;
	void OnDisable() override;

//---- Special Engine Method
																					/// This special method is called by the engine itself after every physics frame.
	static void NotifyCollisions();

//---- Event Handlers
	void OnEvent(const event::GlobalPositionChanged& context) override;
private:
//---- Helper functions
	void ValidateBounds();

//---- Fields
	SDL_FRect m_Bounds;
	uint32_t m_Layer;
	uint32_t m_LayerMask;
	std::array<float, 2> m_RegisteredHBounds{};
	
//---- Static AABB registry
	struct Bound {
		AabbCollider* collider;
		float value;
		bool first;

		bool operator< (const Bound& other) { return value < other.value; }
	};

	static inline std::vector<Bound> s_HBounds{};
	static inline std::set<std::pair<AabbCollider*, AabbCollider*>> s_Collisions{};
}; 
REGISTER_COMPONENT(AabbCollider)

} // !eng