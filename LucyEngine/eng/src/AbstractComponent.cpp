#include "AbstractComponent.h"
#include "Actor.h"

namespace eng {

AbstractComponent::AbstractComponent(Actor& owner) :
    m_Owner(owner) {
}

eng::Actor& AbstractComponent::Owner() {
    return m_Owner;
}

const eng::Actor& AbstractComponent::Owner() const {
    return m_Owner;
}

} // !eng
