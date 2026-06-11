#pragma once

#include <AbstractCommand.h>
#include <PhysicsBody.h>
#include <SpriteController.h>

namespace tron {

class MoveTank : public eng::AbstractCommand {
public:
	//---- Ctor
	MoveTank(glm::vec2 velocity);

	//---- Dtor
	~MoveTank() override;

	//---- Command methods
	bool Execute(eng::Actor& actor) override;
	
private:
	//---- State fields
	const glm::vec2 m_Velocity{};
	const eng::HashedString m_Sprite;
	
	//---- Caching
	static inline std::map<eng::Actor*, eng::PhysicsBody*> s_ActorPhysicsBodies{};
	static inline std::map<eng::Actor*, eng::SpriteController*> s_ActorSpriteControllers{};
};

}