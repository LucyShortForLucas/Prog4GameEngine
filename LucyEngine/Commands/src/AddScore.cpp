#include "AddScore.h"
#include "HpScore.h"
#include "Actor.h"

eng::AddScore::AddScore(int score) :
	m_Score(score) {
}

bool eng::AddScore::Execute(Actor& actor) {
	HpScore* hpScore{ actor.GetComponent<HpScore>() };

	if (!hpScore) return false;

	hpScore->GainScore(m_Score);

	return true;
}