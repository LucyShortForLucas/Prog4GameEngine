#pragma once

#include "Services.h"

namespace eng {

class GameTime final : public service::IGameTime {
public: //---------------------|Time methods|-----------------------------------

	float	DeltaTime() const override;
	float	FixedDeltaTime() const override;
	void	UpdateDeltaTime() override;
	bool	UpdateFixedDeltaTime() override;
	int		MinMilliSecPerFrame() const override;

private: //---------------------------|time fields|-----------------------------

	float m_DeltaTime{};
	int m_MinMillisePerFrame{ 10 };
	int m_FixedMicroInterval{ 20'000 }; // 20 ms
	std::chrono::time_point<std::chrono::steady_clock> m_PreviousTimePoint{ };
	std::chrono::microseconds m_FixedTimeAccumulator{0};
	bool m_Started{ false };
};

}