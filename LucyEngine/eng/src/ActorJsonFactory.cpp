#include "ActorJsonFactory.h"

namespace eng {

//------------ Component Factory

ActorJsonFactory& ComponentJsonFactory::Owner() {
	return m_Owner;
}


//-------------- Actor Factory
																		
ComponentJsonFactory& ActorJsonFactory::AddComponent(const std::string& type) {
	ComponentJsonFactory& compFactory{ m_ComponentFactories.emplace_back(ComponentJsonFactory{*this}) };

	nlohmann::ordered_json& j{ compFactory.m_ComponentJson};
	j["Type"] = type;
	j["Json"] = nlohmann::ordered_json::object();

	return compFactory;
}

ActorJsonFactory& ActorJsonFactory::AddChildActor() {
	ActorJsonFactory& childFactory{ m_ChildFactories.emplace_back(ActorJsonFactory{}) };
	childFactory.m_Parent = this;

	nlohmann::ordered_json& j{ childFactory.m_ActorJson };
	j["Flags"] = 0;
	j["Components"] = nlohmann::ordered_json::array();
	j["Children"] = nlohmann::ordered_json::array();

	return childFactory;
}

ActorJsonFactory& ActorJsonFactory::SetFlag(Actor::Flags flag) {
	unsigned long currentFlags{m_ActorJson.value("Flags", 0ul)};

	m_ActorJson["Flags"] = currentFlags | 1 << static_cast<int>(flag);

	return *this;
}

ActorJsonFactory& ActorJsonFactory::Parent() {
	return *m_Parent;
}

nlohmann::ordered_json& ActorJsonFactory::Build() {
	for (auto& factory : m_ComponentFactories) {
		m_ActorJson["Components"].emplace_back(factory.m_ComponentJson);
	}

	for (auto& factory : m_ChildFactories) {
		m_ActorJson["Children"].emplace_back(factory.Build());
	}

	return m_ActorJson;
}

} // !eng