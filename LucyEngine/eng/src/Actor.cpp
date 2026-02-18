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

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }
}

void Actor::Update() {

    for (auto& child : m_ChildUptrs) {
        child->Update();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Update();
    }
}

void Actor::LateUpdate() {

    for (auto& child : m_ChildUptrs) {
        child->LateUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->LateUpdate();
    }
}

void Actor::FixedUpdate() {

    for (auto& child : m_ChildUptrs) {
        child->FixedUpdate();
    }

    for (auto& compUptr : m_CompUptrs) {
        compUptr->FixedUpdate();
    }

}

void Actor::Render() {

    for (auto& compUptr : m_CompUptrs) {
        compUptr->Render();
    }

    for (auto& child : m_ChildUptrs) {
        child->Render();
    }
}


void Actor::RenderImgui() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->RenderImgui();
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

} // namespace eng

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs) {
    return &lhs == &rhs;
}

