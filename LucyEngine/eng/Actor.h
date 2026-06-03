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
#include <nlohmann/json.hpp>                                                   
#include "Utils.h"															   
#include "AbstractComponent.h"												   
#include "Transform2d.h"													   
#include "EventSource.h"      
                                                                               
namespace eng {																   

class SceneTree; // Forward Declaration
class Actor;

namespace event {

struct ActorDestroyed {
    Actor* actorPtr;
};

struct ActorEnabled {
    Actor* actorPtr;
};

struct ActorDisabled {
    Actor* actorPtr;
};

struct ActorMoved {
    Actor* actorPtr;
    Actor* oldParent;
    Actor* newParent;
};

}
																	               /// The quintessential GameObject. An Actor manages the lifetime and ownership of Components and Child Actors.
class Actor final {
public:																		   
    struct MoveInfo {														   
        Actor* newParentPtr{};												   
        bool keepWorldTransform{};											   
    };				                                                           
                                                                               
//---- Constr/Destr/Copy/Move                                                  
    Actor(SceneTree* sceneTree = nullptr);									   
    ~Actor() = default;														   
                                                                               
    Actor(const Actor&) = delete;											   
    Actor& operator=(const Actor&) = delete;								   
                                                                               
    Actor(Actor&&) = delete;												   
    Actor& operator=(Actor&&) = delete;										   
 
//---- Serialization methods

    nlohmann::ordered_json ToJson();
    void Serialize(const std::string& filePath);
    Actor* DeserializeChild(const nlohmann::json& json);

//---- Child Actor Methods:	
                                                                                   /// Adds a new, empty child actor
    Actor& AddChildActor();													       /// Get all the children of this actor 
    std::vector<Actor*>	GetChildren() const;								       /// Get all the children of this actor and all of their children, recursively
    std::vector<Actor*>	GetAllChildren() const;                                    /// Destroy all children of this actor. Does not destroy children flagged with PreserveOnParentClear
    void ClearChildren();                                                          /// Destroy all children of this actor, regardless of their flags.
    void ForceClearChildren();                                                     
                                                                               
//---- Parent Actor Methods														   
    Actor* GetParent();					
    const Actor* const GetParent() const;
    void SetParent(Actor& newParent, bool keepWorldTransform = true);		        /// Flags this actor to be moved to a new parent at the end of the frame. Note that you cannot reparent the root actor of a scene.                                                                 
                                                                               
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
    const Transform& GetTransform() const;
                                                                               
//---- Flag Enum/Methods
    enum class Flags {	                                                            /// If this flag is set, the actor is 'awake'. Its components' Awake() methods are defferred until the Actor wakes up. The actor is automatically awoken when added to a Scenetree.
        Awake,                                                                      /// If this flag is set, the actor will be destroyed at end of frame   
        Destroyed,																    /// If this flag is set, the actor will be moved to another parent at end of frame											   
        ParentChanged,															    /// Skip this Actor's Update cycle. Disables Update(), LateUpdate(), and FixedUpdate()									   
        NoUpdate,																    /// Skip this Actor's Render cycle. Disable Render().				   
        NoRender,																    /// If not set, runs start methods on its components and sets it at the start of the next frame.								   
        Started,																    /// If set, the actor is considered inactive, Controls Enable() and Disable().													   							
        Enabled,																    /// If set, the actor will start disabled. Disable() will be called in Start(). Setting this flag has no effect if the Started flag has already been set.			   
        DisableOnStart,															    /// If set, this actor will not be destroyed if its parent's ClearChildren() method is called
        PreserveOnParentClear,                                                      /// The size of the Flags enum class 
        SIZE_																       											                                                                   
    };											  
    bool IsFlagged(Flags flag)	const;
                                                                                    /// Awaken this actor, calling Awake() on its components and flagging it as awake.
    void Awaken();                                                                  /// Flag this actor for destruction and call OnDestroy() on its components. Note that the root actor of any given scene cannot be destroyed; you must load a new scene instead.			                                                                       
    void Destroy();																    /// Enable the actor and its children, if disabled. Unsets NoUpdate and	NoRender and calls OnEnable().										                                                                       
    void Enable();															        /// Disable the actor and its children, if enabled. Sets NoUpdate andNoRender and calls OnDisable()										                                                                       
    void Disable();																    /// Set this Actor to start enabled or disabled. By default, actors	are enabled on start.														                                                                       
    void EnableOnStart(bool enable);										        /// Set this Actor to remain even after a call to its parents' Clear() method                                                               
    void PreserveOnParentClear(bool clear = true);

//---- Gameloop Methods 
                                                                                    /// Executed on the first frame this actor is enabled.			                                                                       
    void Start();															        /// Executed every frame if the actor is enabled.				                                                                     
    void Update();															        /// Executed every frame if the actor is enabled, after every other Actor has had their Update method called							                                                                      
    void LateUpdate();														        /// Executed a fixed amount of times per second. May be called more or less than once in the same frame.								                                                                      
    void FixedUpdate();														        /// Executed every frame on RenderComponents during the rendering stage.																                                                                      
    void Render();															        /// Executed every frame on ImguiRenderComponents during the rendering stage.													                                                                      
    void RenderImgui();														        /// Executed every frame as the last logical step of the frame. This function cleans up its children.		 								                                                                      
    void Cleanup();															                                                                     

//---- Event Methods
    void SubscribeActorDestroyed(AbstractEventListener<event::ActorDestroyed>& subject);
    void UnsubscribeActorDestroyed(AbstractEventListener<event::ActorDestroyed>& subject);

    void SubscribeActorMoved(AbstractEventListener<event::ActorMoved>& subject);
    void UnsubscribeActorMoved(AbstractEventListener<event::ActorMoved>& subject);
    
    void SubscribeActorEnabled(AbstractEventListener<event::ActorEnabled>& subject);
    void UnsubscribeActorEnabled(AbstractEventListener<event::ActorEnabled>& subject);

    void SubscribeActorDisabled(AbstractEventListener<event::ActorDisabled>& subject);
    void UnsubscribeActorDisabled(AbstractEventListener<event::ActorDisabled>& subject);

//---- SceneTree methods
    SceneTree* GetSceneTree();
    const SceneTree* const GetSceneTree() const;

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
    SceneTree* m_SceneTreePtr{};

//---- Events
    EventSource<event::ActorDestroyed> m_DestroyEventSource{};
    EventSource<event::ActorMoved> m_MoveEventSource{};
    EventSource<event::ActorEnabled> m_EnabledEventSource{};
    EventSource<event::ActorDisabled> m_DisabledEventSource{};
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
                                                               
    if (IsFlagged(Flags::Awake)) {
        m_CompUptrs.back()->Awake();

        if (IsFlagged(Flags::Enabled))
            m_CompUptrs.back()->OnEnable();

        if (IsFlagged(Flags::Started))
            m_CompUptrs.back()->Start();
    }

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