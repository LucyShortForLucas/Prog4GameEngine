#pragma once

#include "AbstractComponent.h"
#include <glm/vec2.hpp>

#include "Serialization.h"
#include <memory>
#include "EventSource.h"

namespace eng {

struct TransformData {
	glm::vec2 position{};
};

class Transform;

namespace event {

struct GlobalPositionChanged {
	Transform& transform;
};

struct LocalPositionChanged {
	Transform& transform;
};

}

DECL_COMPONENT(Transform, public AbstractComponent)
	DECL_EVENT(GlobalPositionChanged)
	DECL_EVENT(LocalPositionChanged)
public: 
//---- Constructor/Destructor

	Transform(eng::Actor& owner) : AbstractComponent(owner) {};
	~Transform() = default;

	nlohmann::ordered_json Serialize();

//---- Transform Methods 

	void SetLocalPosition(float x, float y);
	void SetLocalPosition(glm::vec2 newPosition);

	void TranslatePosition(glm::vec2 translateVector);

	void SetGlobalPosition(float x, float y);
	void SetGlobalPosition(glm::vec2 newPosition);

	void FlagForGlobalUpdate();

	TransformData const& GetLocal() const;
	TransformData const& GetGlobal() const;

private: 
//---- Transform Fields 
	TransformData m_TransformData{};
	mutable TransformData m_GlobalTransformData{};
	bool m_GlobalNeedsUpdate{};

}; // !TransformComponent
REGISTER_COMPONENT(Transform)
} // !eng


//-----------------------------------------|Operators|------------------------------------

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs);
