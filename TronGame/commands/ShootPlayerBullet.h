#pragma once

#include <AbstractCommand.h>
#include <Turret.h>

namespace tron {

class ShootPlayerBullet : public eng::AbstractCommand {
public:
	~ShootPlayerBullet();

	//---- Command methods
	bool Execute(eng::Actor& actor) override;

private:

	//---- Caching
	static inline std::map<eng::Actor*, tron::Turret*> s_ActorTurrets{};
};

}