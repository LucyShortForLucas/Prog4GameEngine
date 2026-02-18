#include "Transform2d.h"
#include "Actor.h"
#include <iostream>

namespace eng {

void Transform::SetLocalPosition(float x, float y) {
	glm::vec2 f_OldPos{ m_TransformData.position };

	m_TransformData.position.x = x;
	m_TransformData.position.y = y;
}

void Transform::SetLocalPosition(glm::vec2 newPosition) {
	SetLocalPosition(newPosition.x, newPosition.y);
}

void Transform::TranslatePosition(glm::vec2 translateVector) {
	if (translateVector == glm::vec2{}) return;
	SetLocalPosition(m_TransformData.position + translateVector);
}

eng::TransformData const& Transform::GetLocal() const {
	return m_TransformData;
}

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs) {
	return eng::TransformData{ lhs.position + rhs.position };
}

}