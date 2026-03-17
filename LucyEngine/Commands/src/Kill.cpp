#include "Kill.h"

#include "HpScore.h"

bool eng::Kill::Execute(Actor& actor) {
	HpScore* hpScore{actor.GetComponent<HpScore>()};

	if (!hpScore) return false;

	hpScore->ReduceLives(1);

	return true;
}