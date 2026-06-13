#pragma once

#include <AbstractCommand.h>

#include <Actor.h>

#include <GameData.h>

namespace tron {

class NextLevel : public eng::AbstractCommand {
public:
	bool Execute(eng::Actor& actor) override {

		actor.GetComponent<GameData>()->NextLevel();
		return true;
	}
};

}