#include "Actor.h"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace eng {


Actor::Actor() {
    m_TransformPtr = &AddComponent<Transform>();
}

void Actor::Start() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->Start();
    }
}

void Actor::Update() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->Update();
    }
}

void Actor::LateUpdate() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->LateUpdate();
    }
}

void Actor::FixedUpdate() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->FixedUpdate();
    }

}

void Actor::Render() {
    for (auto& compUptr : m_CompUptrs) {
        compUptr->Render();
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

} // namespace eng

bool operator==(const eng::Actor& lhs, const eng::Actor& rhs) {
    return &lhs == &rhs;
}