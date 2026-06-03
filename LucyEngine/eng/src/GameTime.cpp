#include "GameTime.h"

#include <thread>

float eng::GameTime::DeltaTime() const {
	return m_DeltaTime;
}

float eng::GameTime::FixedDeltaTime() const {
	return m_FixedMicroInterval / 1'000'000.f;
}

void eng::GameTime::UpdateDeltaTime() {
	if (!m_Started) {
		m_PreviousTimePoint = std::chrono::steady_clock::now();
		m_Started = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(m_MinMillisePerFrame));
	}

	auto now{ std::chrono::steady_clock::now() };

	m_DeltaTime = std::chrono::duration<float>(now - m_PreviousTimePoint).count();
	m_FixedTimeAccumulator += std::chrono::duration_cast<std::chrono::microseconds>(now - m_PreviousTimePoint);
	m_PreviousTimePoint = now;
}

bool eng::GameTime::UpdateFixedDeltaTime() {
	if (m_FixedTimeAccumulator.count() < m_FixedMicroInterval)
		return  false;
	m_FixedTimeAccumulator -= std::chrono::microseconds{ m_FixedMicroInterval };
	return true;
}

int eng::GameTime::MinMilliSecPerFrame() const {
	return m_MinMillisePerFrame;
}


