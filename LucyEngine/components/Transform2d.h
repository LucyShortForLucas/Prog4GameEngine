#pragma once

#include "AbstractComponent.h"
#include <glm/vec2.hpp>

namespace eng {

struct TransformData {
	glm::vec2 position{};
};


/// <summary>
/// A component that allows Actors to store their local transform and observe their global transform.
/// </summary>
class Transform final : public AbstractComponent {
public: //--------------- Constructor/Destructor/copy/move --------------

	Transform(eng::Actor& owner) : AbstractComponent(owner) {};
	~Transform() = default;

	Transform(const Transform&) = delete;
	Transform& operator=	(const Transform&) = delete;

	Transform(const Transform&&) = delete;
	Transform& operator=	(const Transform&&) = delete;


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

} // !eng


//-----------------------------------------|Operators|------------------------------------

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs);
