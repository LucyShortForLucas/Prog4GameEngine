#include "GameTime.h"

float eng::GameTime::DeltaTime() const {
	return m_DeltaTime;
}

void eng::GameTime::UpdateDeltaTime() {
	m_DeltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_PreviousTimePoint).count();
}

int eng::GameTime::MinMilliSecPerFrame() const {
	return m_MinMillisePerFrame;
}
