#pragma once

#include "Serialization.h"
#include "HashedString.h"
#include "Actor.h"
#include "SceneTree.h"

namespace eng {

DECL_COMPONENT(TagSetter, public AbstractComponent)
public:
	TagSetter(Actor& owner, std::vector<HashedString> tags) : AbstractComponent(owner), m_Tags(tags) {};

	nlohmann::ordered_json Serialize() override;

	void Awake() override;

private:
	std::vector<HashedString> m_Tags;
};
REGISTER_COMPONENT(TagSetter);
}