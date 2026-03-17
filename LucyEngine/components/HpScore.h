#pragma once

#include "EventSource.h"
#include "AbstractComponent.h"

#include "SdbmHash.h"

namespace eng {

namespace eventId {

constexpr unsigned int livesChanged{ make_sdbm_hash("LivesChanged") };
constexpr unsigned int scoreChanged{ make_sdbm_hash("ScoreChanged") };

}

namespace eventContext {

struct LivesChangedContext {
	int oldLives;
	int newLives;
};

struct ScoreChangedContext {
	int oldScore;
	int newScore;
};

}

class HpScore final : public AbstractComponent {
public: //--------------- Constructor/Destructor/copy/move --------------

	HpScore(Actor& owner, int startingLives) : AbstractComponent(owner), m_Lives(startingLives) {};
	~HpScore() = default;

	HpScore(const HpScore&) = delete;
	HpScore& operator=	(const HpScore&) = delete;

	HpScore(HpScore&&) = delete;
	HpScore& operator=	(HpScore&&) = delete;

public: //------------------ Event Source Methods --------------------------

	void Subsribe(AbstractEventListener& subject);
	void UnSubsribe(AbstractEventListener& subject);

public: // ------------------ Setter methods ---------------------------

	void ReduceLives(int damage);
	void GainScore(int score);

private: //----------------- Health and score data ---------------------------

	int m_Lives{};
	int m_Score{};

private: // ---------------- Event Source -------------------------------

	EventSource m_EventSource{};

}; // !HpScore

}