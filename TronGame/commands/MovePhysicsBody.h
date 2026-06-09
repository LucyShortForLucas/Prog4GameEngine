#pragma once

#include <AbstractCommand.h>
#include <PhysicsBody.h>

namespace tron {

class MovePhysicsBody : public eng::AbstractCommand {
public:
	//---- Ctor
	MovePhysicsBody(glm::vec2 velocity) : m_Velocity{ velocity } {};

	//---- Command methods
	bool Execute(eng::Actor& actor) override;
	
private:
	//---- State fields
	const glm::vec2 m_Velocity{};
	
	//---- Fields for caching
	std::map<eng::Actor*, eng::PhysicsBody*> m_ActorPhysicsBodies;
};

}