#include "ClearAndLoad.h"

#include <SceneTree.h>

namespace tron {

bool ClearAndLoad::Execute(eng::Actor& actor) {
	actor.GetSceneTree()->ClearAndLoad(m_ToLoad);
	return true;
}

}