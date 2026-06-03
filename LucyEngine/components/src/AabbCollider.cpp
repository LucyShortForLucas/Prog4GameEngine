#include "AabbCollider.h"
#include <algorithm>
#include "Actor.h"
#include <unordered_set>
#include <future>


namespace eng {

std::unique_ptr<AabbCollider> AabbCollider::Deserialize(Actor& owner, const nlohmann::json& json) {
	SDL_FRect rect{ json.value("Bounds", SDL_FRect{0, 0, 1, 1}) };
	int layer{ json.value("Layer", 0) };
	uint32_t layerMask{ json.value("LayerMask", UINT32_MAX) };
	return std::make_unique<AabbCollider>(owner, rect, layer, layerMask);
}

AabbCollider::AabbCollider(Actor& owner, SDL_FRect bounds, int layer, uint32_t layerMask) :
	AbstractComponent{ owner }, m_Bounds{ bounds }, m_LayerMask{ layerMask } {
	assert(layer <= 31 && layer >= 0);
	assert(bounds.h > 0 && bounds.w > 0);
	m_Layer = 1 << layer;
}

nlohmann::ordered_json AabbCollider::Serialize() {
	return nlohmann::ordered_json();
}

void AabbCollider::OnEnable() {
	//---- Subscribe to events
	Owner().GetTransform().SubscribeGlobalPositionChanged(*this);

	//---- Calculate and cache real horizontal bounds

	const glm::vec2& position{ Owner().GetTransform().GetGlobal().position };

	m_RegisteredHBounds[0] = position.x + m_Bounds.x;
	m_RegisteredHBounds[1] = m_RegisteredHBounds[0] + m_Bounds.w;

	//---- Place horizontal bounds in registry
	if (s_HBounds.empty()) {
		s_HBounds.emplace_back(Bound{ this, m_RegisteredHBounds[0], true });
		s_HBounds.emplace_back(Bound{ this, m_RegisteredHBounds[1], false });
		return;
	}

	bool first = true;
	for (float hBound : m_RegisteredHBounds) {
		auto it{ std::ranges::lower_bound(s_HBounds, hBound, std::less<float>{}, &Bound::value) };
		s_HBounds.emplace(it, Bound{ this, hBound, first });
		first = false;
	}
}

void AabbCollider::OnDisable() {
	//---- Remove Horizontal bounds from Registry
	for (float hBound : m_RegisteredHBounds) {
		auto it{ std::ranges::lower_bound(s_HBounds, hBound, std::less<float>{}, &Bound::value) };

		while (it != s_HBounds.end() && it->collider != this)
			++it;

		if (it != s_HBounds.end())
			s_HBounds.erase(it);
	}

	//---- UnSubscribe from relevant events
	Owner().GetTransform().UnsubscribeGlobalPositionChanged(*this);
}

void AabbCollider::NotifyCollisions() {
	std::unordered_set<AabbCollider*> openSet{};
	std::vector<std::future<std::pair<bool, std::pair<AabbCollider*, AabbCollider*>>>> potentialHits{};


	// Sweep for X hits, then asynchronously check y hits on x hits
	for (Bound bound : s_HBounds) {
		if (bound.first) {
			SDL_FRect rect{ bound.collider ->GetRect() };

			// Asynchronously check Y collisions for any X collisions
			for (AabbCollider* other : openSet) {
				potentialHits.emplace_back(std::async([bound, other, rect]() -> std::pair<bool, std::pair<AabbCollider*, AabbCollider*>> {
					SDL_FRect otherRect{ other->GetRect() };

					auto result{ std::make_pair(true,
						bound.collider < other // Make sure the pair is sorted so collider pairs are always the same
						? std::make_pair(bound.collider, other)
						: std::make_pair(other, bound.collider)
					) };

					if (rect.y > otherRect.y + otherRect.h || rect.y + rect.h < otherRect.y)
						result.first = false; // No hit

					return result;
					}));
			}
			// Add self to the open set
			openSet.emplace(bound.collider);
		}
		else {
			openSet.erase(bound.collider);
		}
	}

	std::set<std::pair<AabbCollider*, AabbCollider*>> collisions;
	for (auto& futureHit : potentialHits) {
		auto hit = futureHit.get();

		if (!hit.first)
			continue;

		collisions.emplace(hit.second);
	}

	std::vector<std::pair<AabbCollider*, AabbCollider*>> collisionStarted{};
	std::vector<std::pair<AabbCollider*, AabbCollider*>> collisionContinues{};
	std::vector<std::pair<AabbCollider*, AabbCollider*>> collisionEnded{};

	auto itCollisions = collisions.begin(), itOldCollisions = s_Collisions.begin();

	while (itCollisions != collisions.end())
		collisionStarted.emplace_back(*itCollisions++);

	while (itOldCollisions != s_Collisions.end())
		collisionEnded.emplace_back(*itOldCollisions++);

	while (itCollisions != collisions.end() && itOldCollisions != s_Collisions.end()) {
		if (!(*itCollisions < *itOldCollisions) || (*itOldCollisions < *itCollisions)) {
			collisionContinues.emplace_back(*itCollisions);
			++itCollisions; ++itOldCollisions;
		}
	}

	for (auto& collision : collisionStarted) {
		collision.first->m_AabbCollisionEnterEvent.Invoke(event::AabbCollisionEnter{ *collision.first, *collision.second });
		collision.second->m_AabbCollisionEnterEvent.Invoke(event::AabbCollisionEnter{ *collision.second, *collision.first });
		collision.first->m_AabbCollisionEvent.Invoke(event::AabbCollision{ *collision.first, *collision.second });
		collision.second->m_AabbCollisionEvent.Invoke(event::AabbCollision{ *collision.second, *collision.first });
	}

	for (auto& collision : collisionContinues) {
		collision.first->m_AabbCollisionEvent.Invoke(event::AabbCollision{ *collision.first, *collision.second });
		collision.second->m_AabbCollisionEvent.Invoke(event::AabbCollision{ *collision.second, *collision.first });
	}

	for (auto& collision : collisionEnded) {
		collision.first->m_AabbCollisionExitEvent.Invoke(event::AabbCollisionExit{ *collision.first, *collision.second });
		collision.second->m_AabbCollisionExitEvent.Invoke(event::AabbCollisionExit{ *collision.second, *collision.first });
	}
}

void AabbCollider::ValidateBounds() {
	const glm::vec2& position{ Owner().GetTransform().GetGlobal().position };

	auto oldRegisteredBounds{ m_RegisteredHBounds };

	m_RegisteredHBounds[0] = position.x + m_Bounds.x;
	m_RegisteredHBounds[1] = m_RegisteredHBounds[0] + m_Bounds.w;

	for (int i{}; i < 2; ++i) {
		// Find the proper bound
		auto it{ std::ranges::lower_bound(s_HBounds, oldRegisteredBounds[i], std::less<float>{}, &Bound::value)};
		while (it != s_HBounds.end() && it->collider != this)
			++it;

		it->value = m_RegisteredHBounds[i];

		// Bubble sort until sorted
		while (it <= s_HBounds.end()-2) {
			auto rightIt{ std::next(it) };
			if (it->value < rightIt->value) break;
			std::swap(*it, *rightIt);
			++it;
			++rightIt;
		}
		while (it != s_HBounds.begin() && it != s_HBounds.end()) {
			auto leftIt{ std::prev(it) };
			if (it->value >= leftIt->value) break;
			std::swap(*it, *leftIt);
			it = leftIt;
		}
	}
}

void AabbCollider::OnEvent(const event::GlobalPositionChanged& context) {
	// Position changed -> both bounds must be re-evaluated.

	ValidateBounds();
}

SDL_FRect AabbCollider::GetBounds()  const {
	return m_Bounds;
}

SDL_FRect AabbCollider::GetRect()  const {
	SDL_FRect result{ m_Bounds };
	const glm::vec2& position{ Owner().GetTransform().GetGlobal().position };
	result.x += position.x;
	result.y += position.y;
	return result;
}

bool AabbCollider::OverlapsWith(const AabbCollider& other) const {
	return OverlapsWith(other.GetRect());
}

bool AabbCollider::OverlapsWith(const SDL_FRect& rect) const {
	const SDL_FRect& myRect{ GetRect() };

	if (myRect.x > rect.x + rect.w ||
		myRect.x + myRect.w < rect.x ||
		myRect.y > rect.y + rect.h ||
		myRect.y + myRect.h < rect.h)
		return false;
	return true;
}

SDL_FRect AabbCollider::OverlappingArea(const AabbCollider& other) const {
	return OverlappingArea(other.GetRect());
}

SDL_FRect AabbCollider::OverlappingArea(const SDL_FRect& rect)  const {
	const SDL_FRect& myRect{ GetRect() };

	SDL_FRect result{};

	result.x = std::max(myRect.x, rect.x);
	result.y = std::max(myRect.y, rect.y);
	result.w = std::min(myRect.x + myRect.w, rect.x + rect.w) - result.x;
	result.h = std::min(myRect.y + myRect.h, rect.y + rect.h) - result.y;

	return result;
}

} // !eng