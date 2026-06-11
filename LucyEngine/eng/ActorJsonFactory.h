#pragma once

#include "Actor.h"
#include <nlohmann/json.hpp>

namespace eng {

class ActorJsonFactory;

class ComponentJsonFactory final {
	friend ActorJsonFactory;
public:
	template <typename T>
	ComponentJsonFactory& AddProperty(const std::string& key, const T& value) {
		m_ComponentJson["Json"][key] = value;

		return *this;
	}
	ActorJsonFactory& Owner();

private:
	//---- Ctor
	ComponentJsonFactory(ActorJsonFactory& owner) : m_Owner(owner) {} // Private constructor, can only be gotten from the actor factory

	//---- data
	nlohmann::ordered_json m_ComponentJson;
	ActorJsonFactory& m_Owner;
};

/// @brief A helper class to dynamically generate JSON objects for actors, either to create custom actor 'blueprints' at runtime, or to facilitate
/// the creation of actor Json files with code. 
class ActorJsonFactory final {
	friend ComponentJsonFactory;
public:
	//---- Json manipulation methods
																					/// Returns reference to Component Factory
	ComponentJsonFactory& AddComponent(const std::string& type);					/// Returns reference to child
	ActorJsonFactory& AddChildActor();												/// Returns reference to self
	ActorJsonFactory& SetFlag(Actor::Flags flag);									/// Returns referene to parent
	ActorJsonFactory& Parent();	
	nlohmann::ordered_json& Build();

private:
	//---- Data
	nlohmann::ordered_json m_ActorJson{};
	std::vector<ComponentJsonFactory> m_ComponentFactories{};
	std::vector<ActorJsonFactory> m_ChildFactories{};
	ActorJsonFactory* m_Parent{};
};

}