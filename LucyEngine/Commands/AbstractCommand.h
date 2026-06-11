#pragma once
#include "Actor.h"

namespace eng {

class AbstractCommand {
public:
	virtual ~AbstractCommand() = default;
	virtual bool Execute(Actor& actor) = 0;
};

}