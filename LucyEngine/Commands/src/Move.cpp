#include "Move.h"
#include "Services.h"

eng::Move::Move(glm::vec2 velocity) :
	m_Velocity(velocity) {}

bool eng::Move::Execute(Actor& target) {
	auto& time{service::gameTime.Get()};
	target.GetTransform().TranslatePosition(m_Velocity * time.DeltaTime());
	return true;
}

