#include "Actor.h"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace eng {


Actor::Actor() {
    m_TransformPtr = &AddComponent<Transform>();
}

void Actor::Start() {
    for (auto& child : m_ChildUptrs) {
        child->Start();
    }

    if (IsFlagged(Flags::Started)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }

    Enable();
    if (IsFlagged(Flags::DisableOnStart)) Disable();

    m_Flags[static_cast<int>(Flags::Started)] = true;
}

void Actor::Update() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->Update();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Update();
    }
}

void Actor::LateUpdate() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->LateUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->LateUpdate();
    }
}

void Actor::FixedUpdate() {
    if (IsFlagged(Flags::NoUpdate)) return;

    for (auto& child : m_ChildUptrs) {
        child->FixedUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->FixedUpdate();
    }

}

void Actor::Render() {
    if (IsFlagged(Flags::NoRender)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Render();
    }

    for (auto& child : m_ChildUptrs) {
        child->Render();
    }
}

void Actor::RenderImgui() {
    if (IsFlagged(Flags::NoRender)) return;

    for (auto& compUptr : m_CompUptrs) {
        compUptr->RenderImgui();
    }

    for (auto& childUptr : m_ChildUptrs) {
        childUptr->RenderImgui();
    }
}

void Actor::Cleanup() {
    for (auto& child : m_ChildUptrs) {

        child->Cleanup();

        // Destroy children if flagged for it
        if (child->IsFlagged(Flags::Destroyed)) std::erase_if(m_ChildUptrs, [&child](const std::unique_ptr<Actor>& childToErase) {
            return childToErase == child;
            });;

        // Move children if flagged for it
        if (child->IsFlagged(Flags::ParentChanged)) {

            // Move child to new parent
            auto& movedChild{ child->m_MoveInfo.newParentPtr->m_ChildUptrs.emplace_back(std::move(child)) };

            // Erase the now empty unique pointer in our old parent
            std::erase_if(m_ChildUptrs, [](const std::unique_ptr<Actor>& childToErase) {
                return childToErase.get() == nullptr; });

            // inform child of new parent
            movedChild->m_ParentPtr = movedChild->m_MoveInfo.newParentPtr;

            // Flag its transform as dirty
            movedChild->GetTransform().FlagForGlobalUpdate();

            // Moving complete
            movedChild->m_MoveInfo.newParentPtr = nullptr;
        }
    }
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

Actor& Actor::AddChildActor() {
    auto& f_NewChild{ m_ChildUptrs.emplace_back(std::make_unique<Actor>()) };

    f_NewChild->m_ParentPtr = this;

    return *m_ChildUptrs.back();
}

std::vector<Actor*> Actor::GetChildren() const {
    std::vector<Actor*> f_Result{};
    for (const auto& childUptr : m_ChildUptrs) {
        f_Result.emplace_back(childUptr.get());
    }
    return f_Result;
}

std::vector<Actor*> Actor::GetAllChildren() const {
    std::vector<Actor*> f_Result = GetChildren();

    for (size_t i{}; i < f_Result.size(); ++i) {
        auto f_ChildChildren{ f_Result[i]->GetChildren() };
        f_Result.insert(f_Result.end(), f_ChildChildren.begin(), f_ChildChildren.end());
    }

    return f_Result;
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

bool Actor::IsFlagged(Flags flag) const {
    return m_Flags.test(static_cast<int>(flag));
}

void Actor::Destroy() {
    if (IsFlagged(Flags::Destroyed) or m_ParentPtr == nullptr) return;
    m_Flags.set(static_cast<int>(Flags::Destroyed));

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

    if (not IsFlagged(Flags::Disabled) and not IsFlagged(Flags::Started)) return;

    m_Flags.reset(static_cast<int>(Flags::Disabled));
    m_Flags.reset(static_cast<int>(Flags::NoRender));
    m_Flags.reset(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnEnable();
    }
}

void Actor::Disable() {
    for (auto& child : m_ChildUptrs) {
        child->Disable();
    }

    if (IsFlagged(Flags::Disabled)) return;

    m_Flags.set(static_cast<int>(Flags::NoRender));
    m_Flags.set(static_cast<int>(Flags::Disabled));
    m_Flags.set(static_cast<int>(Flags::NoUpdate));

    for (auto& compUptr : m_CompUptrs) {
        compUptr->OnDisable();
    }
}

void Actor::EnableOnStart(bool enable) {
    m_Flags[static_cast<int>(Flags::DisableOnStart)] = !enable;
}

} // namespace eng

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs) {
    return &lhs == &rhs;
}

