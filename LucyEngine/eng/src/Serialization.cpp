#include "Serialization.h"
#include "Actor.h"

namespace eng {

std::unordered_map<std::string, CompDeserializeFunc>& CompDeserializeMethods() {
	static std::unordered_map<std::string, CompDeserializeFunc> deserializeMethodsByName{};
	return deserializeMethodsByName;
}

std::unique_ptr<AbstractComponent> DeserializeComponent(Actor& owner, const std::string& name, const nlohmann::json& json) {
	return CompDeserializeMethods().at(name)(owner, json);
}

bool IsComponentRegistered(const std::string& name) {
	return CompDeserializeMethods().contains(name);
}

}