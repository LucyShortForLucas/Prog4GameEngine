#include "HpScore.h"

void eng::HpScore::Subsribe(AbstractEventListener& subject) {
	m_EventSource.Subsribe(subject);
}

void eng::HpScore::UnSubsribe(AbstractEventListener& subject) {
	m_EventSource.UnSubsribe(subject);
}

void eng::HpScore::ReduceLives(int damage) {
	int oldLives = m_Lives;
	m_Lives -= damage;
	m_EventSource.Invoke(eventId::livesChanged, std::make_any<eventContext::LivesChangedContext>(oldLives, m_Lives));
}
	
void eng::HpScore::GainScore(int score) {
	int oldScore = m_Score;
	m_Score -= score;
	m_EventSource.Invoke(eventId::livesChanged, std::make_any<eventContext::LivesChangedContext>(oldScore, m_Score));
}