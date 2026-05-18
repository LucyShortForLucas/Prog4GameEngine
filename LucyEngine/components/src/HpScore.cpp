#include "HpScore.h"

namespace eng {

std::unique_ptr<HpScore> HpScore::Deserialize(Actor& owner, const nlohmann::json& json) {
	return std::make_unique<HpScore>(owner, 0);
}

}

void eng::HpScore::SubscribeLives(AbstractEventListener<event::LivesChanged>& subject) {
	m_LivesChangedEventSource.Subscribe(subject);
}

void eng::HpScore::SubscribeScore(AbstractEventListener<event::ScoreChanged>& subject) {
	m_ScoreChangedEventSource.Subscribe(subject);
}

void eng::HpScore::UnsubscribeLives(AbstractEventListener<event::LivesChanged>& subject) {
	m_LivesChangedEventSource.Unsubscribe(subject);
}

void eng::HpScore::UnsubscribeScore(AbstractEventListener<event::ScoreChanged>& subject) {
	m_ScoreChangedEventSource.Unsubscribe(subject);
}

void eng::HpScore::ReduceLives(int damage) {
	int oldLives = m_Lives;
	m_Lives -= damage;
	m_LivesChangedEventSource.Invoke(event::LivesChanged{oldLives, m_Lives});
}
	
void eng::HpScore::GainScore(int score) {
	int oldScore = m_Score;
	m_Score += score;
	m_ScoreChangedEventSource.Invoke(event::ScoreChanged{ oldScore, m_Score });
}