#pragma once

#pragma once

#include <set>
#include <memory>
#include <vector>
#include <bitset>
#include <cstddef>
#include <unordered_map>
#include <optional>
#include <ranges>
#include <algorithm>

#include "Utils.h"
#include "AbstractComponent.h"
#include "Transform2d.h"

namespace eng {

/// <summary>
/// The quintessential Game Object. An Actor manages the lifetime and ownership of Components and Child Actors.
/// </summary>
class Actor final {
public: //--------------- Constructor/Destructor/copy/move --------------

	Actor();
	~Actor() = default;

	// Actors cannot be copied
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	// Actors can be moved
	Actor(Actor&&) = default;
	Actor& operator=(Actor&&) = default;

public: //-------------- Component Methods -----------------------------


	/// <typeparam name="CompT">The Component type. Must derive from AbstractComponent. Must have valid copy/move constructors and assignment operators in order for the Actor to be able to be cloned</typeparam>
	/// <returns>A reference to the newly added component</returns>
	template <std::derived_from<AbstractComponent> CompT, typename... ArgsT>
	CompT& AddComponent(ArgsT... args);

	/// <returns>
	/// A pointer to this Actor's component of type CompT. Return value is empty if no such component exists.
	/// </returns>
	template <std::derived_from<AbstractComponent> CompT>
	CompT* GetComponent();

	std::vector<AbstractComponent*> GetAbstractComponents();
	
	template <std::derived_from<AbstractComponent> CompT>
	void RemoveComponent();

	Transform& GetTransform();

public: //-------------------- Gameloop Methods --------------------------------

	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();
	void Render();
	void RenderImgui();

private: //----------------------- Component Fields -------------------------------------

	std::vector<std::unique_ptr<AbstractComponent>> m_CompUptrs;

	Transform* m_TransformPtr;

}; // !Actor



//----------------------------------------------------------------------------------------
//---------------------|Template implementation|------------------------------------------
//----------------------------------------------------------------------------------------
#pragma region Template implementation

template<std::derived_from<AbstractComponent> CompT, typename... ArgsT>
CompT& Actor::AddComponent(ArgsT... args) {
	// If a component of type CompT already exists, just return it
	for (auto& compUptr : m_CompUptrs) {
		if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get()); castResult) {
			return *castResult;
		}
	}

	m_CompUptrs.emplace_back(std::make_unique<CompT>(*this, args...));

	return *static_cast<CompT*>(m_CompUptrs.back().get());

}

template<std::derived_from<AbstractComponent> CompT>
inline CompT* Actor::GetComponent() {
	CompT* f_Result{};

	for (auto& compUptr : m_CompUptrs) {
		if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get()); castResult) {
			f_Result = castResult;
			break;
		}
	}

	return f_Result;
}

template<std::derived_from<AbstractComponent> CompT>
inline void Actor::RemoveComponent() {
	m_CompUptrs.erase(
		std::ranges::remove_if(m_CompUptrs, [](std::unique_ptr<AbstractComponent>& compUptr) {
			return dynamic_cast<CompT*>(compUptr.get()) != nullptr;
			}).begin(),
				m_CompUptrs.end()
				);
}

#pragma endregion

} // !namespace eng

//#############################################|Comparison Operator overloads|#################################

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs);