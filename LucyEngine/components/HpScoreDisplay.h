#pragma once

#include "AbstractComponent.h"
#include "TextRenderer.h"
#include "EventSource.h"

namespace eng {

class HpScoreDisplay final : public AbstractComponent, public AbstractEventListener {
public: //--------------- Constructor/Destructor/copy/move --------------

	HpScoreDisplay(Actor& owner) : AbstractComponent(owner) {};
	~HpScoreDisplay() = default;

	HpScoreDisplay(const HpScoreDisplay&) = delete;
	HpScoreDisplay& operator=	(const HpScoreDisplay&) = delete;

	HpScoreDisplay(HpScoreDisplay&&) = delete;
	HpScoreDisplay& operator=	(HpScoreDisplay&&) = delete;

public: //------------------ Event method --------------------------

	void OnEvent(unsigned int id, std::any context) override;

private: //----------------- Text data ----------------------

	std::string m_LiveDisplayText{ "# lives: 3" };
	std::string m_ScoreDisplayText{"Score: 0"};

}; // !Rotator

}