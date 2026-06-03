#include "Transform2d.h"
#include "Actor.h"
#include <iostream>

namespace eng {

nlohmann::ordered_json Transform::Serialize() {
	nlohmann::ordered_json j;

	j["LocalPosition"] = m_TransformData.position;

	return j;
}

std::unique_ptr<Transform> Transform::Deserialize(Actor& owner, const nlohmann::json& json) {
	auto t{ std::make_unique<Transform>(owner) };
	t->SetLocalPosition(json.value("LocalPosition", glm::vec2{}));
	return t;
}


void Transform::SetLocalPosition(float x, float y) {
	m_TransformData.position.x = x;
	m_TransformData.position.y = y;

	m_LocalPositionChangedEvent.Invoke(event::LocalPositionChanged{ *this });
	FlagForGlobalUpdate();
	for (auto child : Owner().GetAllChildren()) {
		child->GetTransform().FlagForGlobalUpdate();
	}
}

void Transform::SetLocalPosition(glm::vec2 newPosition) {
	SetLocalPosition(newPosition.x, newPosition.y);
}

void Transform::TranslatePosition(glm::vec2 translateVector) {
	if (translateVector == glm::vec2{}) return;
	SetLocalPosition(m_TransformData.position + translateVector);
}

void Transform::SetGlobalPosition(float x, float y) {
	SetGlobalPosition(glm::vec2{ x, y });
}

void Transform::SetGlobalPosition(glm::vec2 newPosition) {
	auto f_TranslatePosition{ newPosition - GetGlobal().position };
	TranslatePosition(f_TranslatePosition);
}

void Transform::FlagForGlobalUpdate() {
	m_GlobalNeedsUpdate = true;
	m_GlobalPositionChangedEvent.Invoke(event::GlobalPositionChanged{*this});
}
eng::TransformData const& Transform::GetLocal() const {
	return m_TransformData;
}

eng::TransformData const& Transform::GetGlobal() const {
	if (!Owner().GetParent()) return m_TransformData;
	if (!m_GlobalNeedsUpdate) return m_GlobalTransformData;

	m_GlobalTransformData = GetLocal() + Owner().GetParent()->GetTransform().GetGlobal();

	return m_GlobalTransformData;
}

} 

eng::TransformData operator+(const eng::TransformData& lhs, const eng::TransformData& rhs) {
	return eng::TransformData{ lhs.position + rhs.position };

}