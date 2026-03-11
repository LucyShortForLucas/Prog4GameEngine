#pragma once
#include "Actor.h"

namespace eng {

class AbstractCommand {
public:
	virtual bool Execute(Actor& actor) = 0;
};

}