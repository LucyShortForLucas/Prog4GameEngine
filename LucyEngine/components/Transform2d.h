#pragma once

#include "AbstractComponent.h"
#include <glm/vec2.hpp>

#include "Serialization.h"
#include <memory>

namespace eng {

struct TransformData {
	glm::vec2 position{};
};

DECL_COMPONENT(Transform, public AbstractComponent)
public: //--------------- Constructor/Destructor --------------

	Transform(eng::Actor& owner) : AbstractComponent(owner) {};
	~Transform() = default;

	nlohmann::ordered_json Serialize();

public: //--------------- Transform Methods --------------


	void SetLocalPosition(float x, float y);
	void SetLocalPosition(glm::vec2 newPosition);

	void TranslatePosition(glm::vec2 translateVector);

	void SetGlobalPosition(float x, float y);
	void SetGlobalPosition(glm::vec2 newPosition);

	void FlagForGlobalUpdate();

	TransformData const& GetLocal() const;
	TransformData const& GetGlobal();

private: //--------------------------- Transform Fields ----------------------------
	TransformData m_TransformData{};
	TransformData m_GlobalTransformData{};
	bool m_GlobalNeedsUpdate{};

}; // !TransformComponent
REGISTER_COMPONENT(Transform)
} // !eng


//-----------------------------------------|Operators|------------------------------------

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs);
