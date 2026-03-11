#include "Actor.h"

class AbstractCommand {
public:
	void Execute(Actor& actor) = 0;
};