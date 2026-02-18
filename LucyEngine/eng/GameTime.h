#pragma once

#include "Services.h"

namespace eng {

class GameTime final : public service::IGameTime {
public: //---------------------|Time methods|-----------------------------------

	float	DeltaTime() const override;
	void	UpdateDeltaTime() override;
	int		MinMilliSecPerFrame() const override;

private: //---------------------------|time fields|-----------------------------

	float m_DeltaTime{};
	int m_MinMillisePerFrame{ 10 };
	std::chrono::time_point<std::chrono::steady_clock> m_PreviousTimePoint{ std::chrono::steady_clock::now()};
};

}