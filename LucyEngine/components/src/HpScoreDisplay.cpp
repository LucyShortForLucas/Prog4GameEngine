#include "HpScoreDisplay.h"
#include "HpScore.h"
#include "Actor.h"

void eng::HpScoreDisplay::UpdateDisplay() {
	auto textRenderPtr{ Owner().GetComponent<TextRenderer>() };
	if (!textRenderPtr) return;

	textRenderPtr->SetText(m_LiveDisplayText + " " + m_ScoreDisplayText);
}

void eng::HpScoreDisplay::OnEvent(const event::LivesChanged& event) {
	m_LiveDisplayText = "# lives: " + std::to_string(event.newLives);
}

void eng::HpScoreDisplay::OnEvent(const event::ScoreChanged& event) {
	m_ScoreDisplayText = "Score: " + std::to_string(event.newScore);
}