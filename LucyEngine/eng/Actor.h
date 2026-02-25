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
public: //--------------- Helper structs -----------------------------
	struct MoveInfo {
		Actor* newParentPtr{};
		bool keepWorldTransform{};
	};

public: //--------------- Constructor/Destructor/copy/move --------------

	Actor();
	~Actor() = default;

	// Actors cannot be copied
	Actor(const Actor&) = delete;
	Actor& operator=(const Actor&) = delete;

	Actor(Actor&&) = delete;
	Actor& operator=(Actor&&) = delete;

public: //-------------- Child Actor Methods ------------------
	Actor& AddChildActor();

	std::vector<Actor*>	GetChildren() const;

	std::vector<Actor*>	GetAllChildren() const;

public: //---------------|Parent Actor Methods|-------------------------


	Actor* GetParent();

	/// @brief Flags this actor to be moved to a new parent at the end of the frame. Note that you cannot reparent the root actor of a scene.
	void SetParent(Actor& newParent, bool keepWorldTransform = true);

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

public: //---------------------|Flag Enum/Methods|-----------------------------
	enum class Flags {
		/// If this flag is set, the actor will be destroyed at end of frame
		Destroyed,
		/// If this flag is set, the actor will be moved to another parent at end of frame
		ParentChanged,
		/// Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()
		NoUpdate,
		/// Skip this Actor's Render cycle. Disable Render().
		NoRender,
		/// If not set, runs start methods on its components and sets it at the start of the next frame.
		Started,
		/// If set, the actor is considered inactive, Controls Enable() and Disable().
		Disabled,
		/// If set, the actor will start disabled. Disable() will be called in Start(). Setting this flag has no effect if the Started flag has already been set.
		DisableOnStart,

		SIZE_
	};

	bool IsFlagged(Flags flag)	const;

	/// <summary>
	/// Flag this actor for destruction and call OnDestroy() on its components. Note that the root actor of any given scene cannot be destroyed; you must load a new scene instead.
	/// </summary>
	void Destroy();

	/// <summary>
	/// Enable the actor and its children, if disabled. Unsets NoUpdate and NoRender and calls OnEnable().
	/// </summary>
	void Enable();

	/// <summary>
	/// Disable the actor and its children, if enabled. Sets NoUpdate and NoRender and calls OnDisable()
	/// </summary> 
	void Disable();

	/// <summary>
	/// Set this Actor to start enabled or disabled. By default, actors are enabled on start.
	/// </summary>
	void EnableOnStart(bool enable);

public: //-------------------- Gameloop Methods --------------------------------

	/// @brief Executed on the first frame this actor is enabled.
	void Start();
	/// @brief Executed every frame if the actor is enabled.
	void Update();
	/// @brief Executed every frame if the actor is enabled, after every other Actor has had their Update method called
	void LateUpdate();
	/// @brief Executed a fixed amount of times per second. May be called more or less than once in the same frame.
	void FixedUpdate();
	/// @brief Executed every frame on RenderComponents during the rendering stage.
	void Render();
	/// @brief Executed every frame on ImguiRenderComponents during the rendering stage.
	void RenderImgui();
	/// @brief Executed every frame as the last logical step of the frame. This function cleans up its children.
	void Cleanup();

private: //-------------------- Child/Parent Actor Fields -----------------------------------

	Actor* m_ParentPtr{};

	std::vector<std::unique_ptr<Actor>> m_ChildUptrs{};

	MoveInfo m_MoveInfo{};

private: //----------------------- Component Fields -------------------------------------

	std::vector<std::unique_ptr<AbstractComponent>> m_CompUptrs;

	Transform* m_TransformPtr;

private: //----------------------- Metadata Fields -------------------------------------------

	std::bitset<static_cast<int>(Flags::SIZE_)> m_Flags{};
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

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs);