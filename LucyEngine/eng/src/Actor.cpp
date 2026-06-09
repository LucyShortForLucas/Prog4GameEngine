#include "Actor.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include "Serialization.h"
#include "SceneTree.h"

namespace eng {


Actor::Actor(SceneTree* sceneTreePtr) :
    m_SceneTreePtr(sceneTreePtr),
    m_TransformPtr{ &AddComponent<Transform>() }  {
}

void Actor::Start() {
    for (auto& child : m_ChildUptrs) {
        child->Start();
    }

    if (IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }

    m_Flags[static_cast<int>(Flags::Started)] = true;

    Enable();
    if (IsFlagged(Flags::DisableOnStart)) 
        Disable();
}

void Actor::Update() {
    if (IsFlagged(Flags::NoUpdate) or not IsFlagged(Flags::Started)) return;

    for (auto& child : m_ChildUptrs) {
        child->Update();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Update();
    }
}

void Actor::LateUpdate() {
    if (IsFlagged(Flags::NoUpdate) or not IsFlagged(Flags::Started)) return;

    for (auto& child : m_ChildUptrs) {
        child->LateUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->LateUpdate();
    }
}

void Actor::FixedUpdate() {
    if (IsFlagged(Flags::NoUpdate) or not IsFlagged(Flags::Started)) return;

    for (auto& child : m_ChildUptrs) {
        child->FixedUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->FixedUpdate();
    }

}

void Actor::Render() {
    if (IsFlagged(Flags::NoRender) or not IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Render();
    }

    for (auto& child : m_ChildUptrs) {
        child->Render();
    }
}

void Actor::RenderImgui() {
    if (IsFlagged(Flags::NoRender) or not IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->RenderImgui();
    }

    for (auto& childUptr : m_ChildUptrs) {
        childUptr->RenderImgui();
    }
}

void Actor::Cleanup() {
    if (not IsFlagged(Flags::Started)) return;

    for (auto& child : m_ChildUptrs) {

        child->Cleanup();

        // Move children if flagged for it
        if (child->IsFlagged(Flags::ParentChanged)) {

            // Move child to new parent
            auto& movedChild{ 
                child->m_MoveInfo.newParentPtr->m_ChildUptrs.emplace_back(
                    std::move(child)
                ) 
            };

            // Erase the now empty unique pointer in our old parent
            std::erase_if(m_ChildUptrs, [](const std::unique_ptr<Actor>& childToErase) {
                return childToErase.get() == nullptr; });

            // inform child of new parent
            movedChild->m_ParentPtr = movedChild->m_MoveInfo.newParentPtr;

            // Flag its transform as dirty
            movedChild->GetTransform().FlagForGlobalUpdate();

            // Moving complete
            movedChild->m_MoveInfo.newParentPtr = nullptr;

            movedChild->m_ActorMovedEvent.Invoke(event::ActorMoved{movedChild.get(), this, movedChild->GetParent()});
        }
    }

    // Erase children that are flagged for it
    std::erase_if(m_ChildUptrs, [](const std::unique_ptr<Actor>& child) {
        return child->IsFlagged(Flags::Destroyed);
        });
}

std::vector<AbstractComponent*> Actor::GetAbstractComponents() {
    std::vector<AbstractComponent*> f_Result{};

    for (auto& compUptr : m_CompUptrs) {
        f_Result.emplace_back(compUptr.get());
    }

    return f_Result;
}

Transform& Actor::GetTransform() {
    return *m_TransformPtr;
}

const Transform& Actor::GetTransform() const {
    return *m_TransformPtr;
}

Actor& Actor::AddChildActor() {
    auto& newChild{ m_ChildUptrs.emplace_back(std::make_unique<Actor>(m_SceneTreePtr)) };

    newChild->m_ParentPtr = this;

    return *m_ChildUptrs.back();
}

std::vector<Actor*> Actor::GetChildren() const {
    std::vector<Actor*> result{};
    for (const auto& childUptr : m_ChildUptrs) {
        result.emplace_back(childUptr.get());
    }
    return result;
}

std::vector<Actor*> Actor::GetAllChildren() const {
    std::vector<Actor*> result = GetChildren();

    for (size_t i{}; i < result.size(); ++i) {
        auto childChildren{ result[i]->GetChildren() };
        result.insert(result.end(), childChildren.begin(), childChildren.end());
    }

    return result;
}

void Actor::SetParent(Actor& newParent, bool keepWorldTransform) {
    // Assert that this call is valid
    assert(m_ParentPtr and "Root Actors cannot be assigned the child of another actor.");
    assert(&newParent != this and "An actor cannot be its own parent");

    auto f_Children{ GetAllChildren() };
    while (!f_Children.empty()) {
        assert(f_Children.back() != this and "An actor cannot be a child of its children");
        f_Children.pop_back();
    }

    // Can't move to destroyed actor
    if (newParent.IsFlagged(Flags::Destroyed)) return;

    m_Flags.set(static_cast<int>(Flags::ParentChanged));
    m_MoveInfo.newParentPtr = &newParent;
    m_MoveInfo.keepWorldTransform = keepWorldTransform;
}

Actor* eng::Actor::GetParent() {
    return m_ParentPtr;
}

const Actor* const eng::Actor::GetParent() const {
    return m_ParentPtr;
}

bool Actor::IsFlagged(Flags flag) const {
    return m_Flags.test(static_cast<int>(flag));
}

void Actor::Destroy() {
    if (IsFlagged(Flags::Destroyed) or m_ParentPtr == nullptr) return;
    m_Flags.set(static_cast<int>(Flags::Destroyed));

    m_ActorDestroyedEvent.Invoke(event::ActorDestroyed{this});

    for (auto& child : m_ChildUptrs) {
        child->Destroy();
    }

    Disable();

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDestroy();
    }
}

void Actor::Enable() {
     for (auto& child : m_ChildUptrs) {
        child->Enable();
    }

    if (IsFlagged(Flags::Enabled) or not IsFlagged(Flags::Started)) return;

    m_Flags.set(static_cast<int>(Flags::Enabled));
    m_Flags.reset(static_cast<int>(Flags::NoRender));
    m_Flags.reset(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnEnable();
    }

    m_ActorEnabledEvent.Invoke(event::ActorEnabled{this});
}

void Actor::Disable() {
    for (auto& child : m_ChildUptrs) {
        child->Disable();
    }

    if (not IsFlagged(Flags::Enabled)) return;

    m_Flags.set(static_cast<int>(Flags::NoRender));
    m_Flags.reset(static_cast<int>(Flags::Enabled));
    m_Flags.set(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDisable();
    }

    m_ActorDisabledEvent.Invoke(event::ActorDisabled{ this });
}

void Actor::EnableOnStart(bool enable) {
    m_Flags[static_cast<int>(Flags::DisableOnStart)] = !enable;
}

void Actor::PreserveOnParentClear(bool clear) {
    m_Flags[static_cast<int>(Flags::PreserveOnParentClear)] = clear;

}

nlohmann::ordered_json Actor::ToJson() {
    nlohmann::ordered_json j{};

    j["Flags"] = m_Flags.to_ulong();

    for (auto& comp : GetAbstractComponents()) {
        if (not IsComponentRegistered(comp->TypeName())) continue;

        j["Components"].emplace_back(nlohmann::ordered_json{
            {"Type", comp->TypeName()},
            {"Json", comp->Serialize()}
            });
    }

    for (auto& child : GetChildren()) {
        j["Children"].emplace_back(child->ToJson());
    }

    return j;
}

void Actor::Serialize(const std::string& filePath) {
    std::ofstream file(filePath, std::ios::out | std::ios::in | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << " for writing.\n";
        return;
    }

    file << ToJson().dump(4);

    if (!file) {
        std::cerr << "Error: Failed to write to file " << filePath << ".\n";
    }
}

Actor* Actor::DeserializeChild(const nlohmann::json& json) {
    auto& child{ AddChildActor() };

    if (json.empty())
        return &child;

    child.m_Flags = json.value("Flags", 0ul);

    if (json.contains("Components")) {
        for (auto& [key, compJson] : json["Components"].items()) {
            if (!compJson.contains("Type"))
                continue;

            if (!IsComponentRegistered(compJson["Type"]))
                continue;

            if (compJson["Type"] == "Transform") {
                // Special case for Transforms. The default one has to be removed and the m_TransformPtr has to be updated. This solution isn't entirely elegant, but good enough.
                child.RemoveComponent<Transform>();
                child.m_TransformPtr = dynamic_cast<Transform*>(child.m_CompUptrs.emplace_back(DeserializeComponent(child, compJson["Type"], compJson["Json"])).get());
                continue;
            }
            
            auto j{ compJson.value("Json", nlohmann::json::object()) };
            child.m_CompUptrs.emplace_back(DeserializeComponent(child, compJson["Type"], j));
        }
    }

    if (!json.contains("Children")) 
        return &child;

    for (auto& [key, childJson] : json["Children"].items()) {
        child.DeserializeChild(childJson);
    }

    return &child;
}

void eng::Actor::ClearChildren() {
    for (auto child : GetChildren()) {
        if (child->IsFlagged(Flags::PreserveOnParentClear))
            continue;
        child->Destroy();
    }
}

void eng::Actor::ForceClearChildren() {
    for (auto child : GetChildren()) 
        child->Destroy();
}

SceneTree* eng::Actor::GetSceneTree() {
    return m_SceneTreePtr;
}

const SceneTree* const eng::Actor::GetSceneTree() const {
    return m_SceneTreePtr;
}

} // namespace eng

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs) {
    return &lhs == &rhs;
}

