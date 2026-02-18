#include "GameTime.h"

float eng::GameTime::DeltaTime() const {
	return m_DeltaTime;
}

void eng::GameTime::UpdateDeltaTime() {
	auto now{ std::chrono::steady_clock::now() };
	m_DeltaTime = std::chrono::duration<float>(now - m_PreviousTimePoint).count();
	m_PreviousTimePoint = now;
}

int eng::GameTime::MinMilliSecPerFrame() const {
	return m_MinMillisePerFrame;
}
