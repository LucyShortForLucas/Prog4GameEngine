#include "RepoolAfterNSeconds.h"

#include <Services.h>
#include <SceneTree.h>
#include <Turret.h>

namespace tron {

std::unique_ptr<RepoolAfterNSeconds> RepoolAfterNSeconds::Deserialize(eng::Actor& owner, const nlohmann::json& json) {
	return std::make_unique<RepoolAfterNSeconds>(owner, json.value("SecondsToLive", 0.5f), eng::runtime_sdbm_hash(json.value("Tag", "Tag")));
}

void RepoolAfterNSeconds::Update() {
	m_TimeLived += eng::service::gameTime.Get().DeltaTime();

	if (m_TimeLived < m_TimeToLive)
		return;

	m_TimeLived = 0;

	Owner().Disable();

	Owner().GetSceneTree()->TagActor(m_Tag, &Owner());
}

}