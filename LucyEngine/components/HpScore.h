#pragma once

#include "EventSource.h"
#include "AbstractComponent.h"

#include "SdbmHash.h"
#include "Serialization.h"

namespace eng {

namespace event {

struct LivesChanged {
	int oldLives;
	int newLives;
};

struct ScoreChanged {
	int oldScore;
	int newScore;
};

}

DECL_COMPONENT(HpScore)
public: //--------------- Constructor/Destructor/copy/move --------------

	HpScore(Actor& owner, int startingLives) : AbstractComponent(owner), m_Lives(startingLives) {};
	~HpScore() = default;

public: //------------------ Event Source Methods --------------------------

	void SubscribeLives(AbstractEventListener<event::LivesChanged>& subject);
	void SubscribeScore(AbstractEventListener<event::ScoreChanged>& subject);
	void UnsubscribeLives(AbstractEventListener<event::LivesChanged>& subject);
	void UnsubscribeScore(AbstractEventListener<event::ScoreChanged>& subject);

public: // ------------------ Setter methods ---------------------------

	void ReduceLives(int damage);
	void GainScore(int score);

private: //----------------- Health and score data ---------------------------

	int m_Lives{};
	int m_Score{};

private: // ---------------- Event Source -------------------------------

	EventSource<event::LivesChanged> m_LivesChangedEventSource{};
	EventSource<event::ScoreChanged> m_ScoreChangedEventSource{};

}; // !HpScore

}