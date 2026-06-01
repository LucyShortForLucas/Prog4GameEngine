#include "TagSetter.h"

#include <ranges>
#include <vector>

namespace eng {

nlohmann::ordered_json eng::TagSetter::Serialize() {
	nlohmann::ordered_json j{};

	std::vector<std::string> tags;

	std::ranges::transform(
		m_Tags,
		std::back_inserter(tags),
		&HashedString::text
	);

	j["Tags"] = tags;

	return j;
}

std::unique_ptr<TagSetter> TagSetter::Deserialize(Actor& owner, const nlohmann::json& json) {
	std::vector<std::string> tags{ json.value("Tags", std::vector<std::string>{}) };
	std::vector<HashedString> hashedTags{};
	hashedTags.reserve(tags.size());

	for (auto& tag : tags) {
		hashedTags.emplace_back(tag);
	}

	return std::make_unique<TagSetter>(owner, hashedTags);
}

void TagSetter::Awake() {
	for (auto& tag : m_Tags) {
		Owner().GetSceneTree()->TagActor(tag.hash, &Owner());
	}
}

} // !eng