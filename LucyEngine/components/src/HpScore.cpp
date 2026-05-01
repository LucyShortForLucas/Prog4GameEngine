#include "HpScore.h"

void eng::HpScore::Subscribe(AbstractEventListener& subject) {
	m_EventSource.Subscribe(subject);
}

void eng::HpScore::Unsubscribe(AbstractEventListener& subject) {
	m_EventSource.Unsubscribe(subject);
}

void eng::HpScore::ReduceLives(int damage) {
	int oldLives = m_Lives;
	m_Lives -= damage;
	m_EventSource.Invoke(eventId::livesChanged, std::make_any<eventContext::LivesChangedContext>(oldLives, m_Lives));
}
	
void eng::HpScore::GainScore(int score) {
	int oldScore = m_Score;
	m_Score += score;
	m_EventSource.Invoke(eventId::scoreChanged, std::make_any<eventContext::ScoreChangedContext>(oldScore, m_Score));
}