#include "HpScoreDisplay.h"
#include "HpScore.h"
#include "Actor.h"

void eng::HpScoreDisplay::OnEvent(unsigned int id, std::any context) {
	switch (id) {
	case eventId::livesChanged:
		m_LiveDisplayText = "# lives: " + std::any_cast<eventContext::LivesChangedContext>(context).newLives;
		break;

	case eventId::scoreChanged:
		m_LiveDisplayText = "Score: " + std::any_cast<eventContext::ScoreChangedContext>(context).newScore;
		break;
	}

	auto textRenderPtr{ Owner().GetComponent<TextRenderer>() };
	if (!textRenderPtr) return;

	textRenderPtr->SetText(m_LiveDisplayText + "/n" + m_ScoreDisplayText);
}