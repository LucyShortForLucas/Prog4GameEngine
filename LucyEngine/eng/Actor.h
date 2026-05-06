#pragma once																   // 
                                                                               //	The Actor class is the main building block of scenes within any 
#include <set>																   // game created in this engine. It is equivalent to GameObjects in 
#include <memory>															   // Unity and Actors in Unreal.
#include <vector>															   //  
#include <bitset>															   //	At its core, all this class does is manage two sets of other 
#include <cstddef>															   // objects: Components and other Actors. Components are the actual meat
#include <unordered_map>													   // of an entity's behaviour; they hold virtually all game logic. The 
#include <optional>															   // other Actors an Actor manages are its children. They inherit its 
#include <ranges>															   // transform data and are destroyed when their parent is destroyed.
#include <algorithm>														   //  
                                                                               //	Most of the time an Actor represents a single 'entity' inside of 
#include "Utils.h"															   // a game scene, however every scene also has a single, special
#include "AbstractComponent.h"												   // actor: its Root.
#include "Transform2d.h"													   //  
                                                                               // ...]
namespace eng {																   // 
                                                                               // 										
class Actor final {																	   /// The quintessential GameObject. An Actor manages the lifetime and ownership of Components and Child Actors.
public:																		   // [...
    struct MoveInfo {														   //	
        Actor* newParentPtr{};												   //   The 'Root Actor' or 'Scene Root' is the top-level actor that
        bool keepWorldTransform{};											   // ultimately owns all other actors in a scene. It is the only actor
    };				                                                           // in any given scene that does not have a parent of its own. In this
                                                                               // way, reasoning about the Root Actor is akin to reasoning about the
//---- Constr/Destr/Copy/Move                                                  // scene as a whole.
    Actor();																   //
    ~Actor() = default;														   //	The reason for this design choice is that by forcing only a single 
                                                                               // 'free actor' to exist in any given scene, there is no real need for
    Actor(const Actor&) = delete;											   // a 'scene' at all; a scene is conceptually equivalent to a Root Actor.
    Actor& operator=(const Actor&) = delete;								   // This removes the need for any sort of scene class or global scene
                                                                               // manager, and makes reasoning about a scene the same as reasoning
    Actor(Actor&&) = delete;												   // about an actor, simplifying development. 
    Actor& operator=(Actor&&) = delete;										   //====================================================================
                                                                               
//---- Child Actor Methods:													   
    Actor& AddChildActor();													   
    std::vector<Actor*>	GetChildren() const;								    
    std::vector<Actor*>	GetAllChildren() const;								    
                                                                               
//---- Parent Actor Methods														   
    Actor* GetParent();															    /// Flags this actor to be moved to a new parent at the end of the frame. Note that you cannot reparent the root actor of a scene.
    void SetParent(Actor& newParent, bool keepWorldTransform = true);		                                                                         
                                                                               
//---- Component Methods													   
    template <std::derived_from<AbstractComponent> CompT, typename... ArgsT>   
    CompT& AddComponent(ArgsT... args);												/// <returns>A pointer to this Actor's component of type CompT. Return value is empty if no such component exists.</returns>															   
    template <std::derived_from<AbstractComponent> CompT>					   
    CompT* GetComponent();												   
    std::vector<AbstractComponent*> GetAbstractComponents();				   
    template <std::derived_from<AbstractComponent> CompT>					   
    void RemoveComponent();													   

//---- Transform methods
    Transform& GetTransform();												   
                                                                               
//---- Flag Enum/Methods
    enum class Flags {															    /// If this flag is set, the actor will be destroyed at end of frame   
        Destroyed,																    /// If this flag is set, the actor will be moved to another parent at end of frame											   
        ParentChanged,															    /// Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()									   
        NoUpdate,																    /// Skip this Actor's Render cycle. Disable Render().				   
        NoRender,																    /// If not set, runs start methods on its components and sets it at the start of the next frame.								   
        Started,																    /// If set, the actor is considered inactive, Controls Enable() and Disable().													   
        Disabled,																    /// If set, the actor will start disabled. Disable() will be called in Start(). Setting this flag has no effect if the Started flag has already been set.										   
        DisableOnStart,															    /// The size of the Flags enum class											                                                                   
        SIZE_																  
    };											  
    bool IsFlagged(Flags flag)	const;											    /// Flag this actor for destruction and call OnDestroy() on its components. Note that the root actor of any given scene cannot be destroyed; you must load a new scene instead.										                                                                       
    void Destroy();																    /// Enable the actor and its children, if disabled. Unsets NoUpdate and	NoRender and calls OnEnable().										                                                                       
    void Enable();															        /// Disable the actor and its children, if enabled. Sets NoUpdate andNoRender and calls OnDisable()										                                                                       
    void Disable();																    /// Set this Actor to start enabled or disabled. By default, actors	are enabled on start.														                                                                       
    void EnableOnStart(bool enable);										                                                                       
                                                                                                                                                   
//---- Gameloop Methods 
                                                                                    /// @brief Executed on the first frame this actor is enabled.			                                                                      
    void Start();															        /// Executed every frame if the actor is enabled.				                                                                      
    void Update();															        /// Executed every frame if the actor is enabled, after every other Actor has had their Update method called							                                                                      
    void LateUpdate();														        /// Executed a fixed amount of times per second. May be called more or less than once in the same frame.								                                                                      
    void FixedUpdate();														        /// Executed every frame on RenderComponents during the rendering stage.																                                                                      
    void Render();															        /// Executed every frame on ImguiRenderComponents during the rendering stage.													                                                                      
    void RenderImgui();														        /// Executed every frame as the last logical step of the frame. This function cleans up its children.										                                                                      
    void Cleanup();															                                                                      

private:
//---- Child/Parent Actor Fields                                                               
                                                                                                                                                  
    Actor* m_ParentPtr{};													                                                                      
                                                                                                                                                  
    std::vector<std::unique_ptr<Actor>> m_ChildUptrs{};						                                                                      
                                                                                                                                                  
    MoveInfo m_MoveInfo{};													                                                                      
                                                                                                                                                  
//---- Component Fields                                                                
                                                                                                                                                  
    std::vector<std::unique_ptr<AbstractComponent>> m_CompUptrs;			                                                                      
                                                                                                                                                  
    Transform* m_TransformPtr;												                                                                      
                                                                                                                                                  
//---- Metadata Fields                                                            
                                                                                                                                                  
    std::bitset<static_cast<int>(Flags::SIZE_)> m_Flags{};					                                                                      
}; // !Actor																                                                                      
                                                                                                                                                  
                                                                                                                                                  
//--------------------- Template implementation -------------------------------                                                                   
#pragma region Template implementation										                                                                      
                                                                                                                                                  
template<std::derived_from<AbstractComponent> CompT, typename... ArgsT>		                                                                      
CompT& Actor::AddComponent(ArgsT... args) {									                                                                      
    // If a component of type CompT already exists, just return it			                                                                      
    for (auto& compUptr : m_CompUptrs) {									                                                                      
        if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get());		                                                                      
            castResult) {													                                                                      
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
        if (CompT* castResult = dynamic_cast<CompT*>(compUptr.get()); 		                                                                      
            castResult) {													                                                                      
            f_Result = castResult;											                                                                      
            break;															                                                                      
        }																	                                                                      
    }																		                                                                      
                                                                                                                                                  
    return f_Result;														                                                                      
}																			                                                                      
                                                                                                                                                  
template<std::derived_from<AbstractComponent> CompT>						                                                                      
inline void Actor::RemoveComponent() {										                                                                      
    m_CompUptrs.erase(														                                                                      
        std::ranges::remove_if(m_CompUptrs, 								                                                                      
            [](std::unique_ptr<AbstractComponent>& compUptr) {				                                                                      
            return dynamic_cast<CompT*>(compUptr.get()) != nullptr;			                                                                      
            }).begin(),														                                                                      
                m_CompUptrs.end()											                                                                      
                );															                                                                      
}																			                                                                      
                                                                                                                                                  
#pragma endregion															                                                                      
                                                                                                                                                  
} // !namespace eng															                                                                      
                                                                                                                                                  
bool operator==(const eng::Actor& lhs, const eng::Actor& rhs);				                                                                      