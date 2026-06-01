#pragma once

#include <memory>
#include <unordered_map>
#include "AbstractComponent.h"
#include <functional>
#include <concepts>
#include <glm/glm.hpp>
#include <SDL3/SDl.h>
#include "Utils.h"
#include <nlohmann/json.hpp>
#include "HashedString.h"

namespace eng {
class Actor;
}

namespace eng {

//---- Macros ----

/// @brief
/// A macro to act as a 'header' for Component Declarations. Defines 'x' as the name of the component and guarantees
/// all the serialization methods are set up correctly. Also deletes the copy/move constructors (which should happen
/// for all components anyway). Declares the static method x::Deserialize(Actor& owner, const nlohmann::json& json),
/// which must be implemented in source. Leaves the class body open with private access modifier.
/// 
/// First argument is the name of the component type, followed by one or more other classes to inherit from. All Components must inherit from AbstractComponent or a derived class thereof.
/// 
#define DECL_COMPONENT(x, ...) class x final : __VA_ARGS__ { \
public: \
    static std::unique_ptr<x> Deserialize(Actor& owner, const nlohmann::json& json); \
    const std::string& TypeName() override { static const std::string s{#x}; return s; }; \
	x(const x&) = delete; \
	x& operator=(const x&) = delete; \
	x(x&&) = delete; \
	x& operator=(x&&) = delete; \
private: \

/// @brief
/// A macro to handle the registration of the component's 
#define REGISTER_COMPONENT(x) namespace { \
	[[maybe_unused]] static inline const bool x##_registered = [] { \
		eng::RegisterComponentType<x>(#x); \
		return true; \
    }(); \
}

//---- Typedefs ----

using CompDeserializeFunc = std::function<std::unique_ptr<AbstractComponent>(Actor&, const nlohmann::json&)>;


//---- Concepts ----

template <typename T>
concept RegistrableComponent = std::derived_from<T, AbstractComponent> &&
	requires (T a, Actor& actor, const nlohmann::json& json) {
		{ T::Deserialize(actor, json) } -> std::convertible_to<std::unique_ptr<T>>;
};

// 'singleton' like function to ensure correct order of initialization when this map is accessed during the intialization of static variables (i.e. when registering components on program startup). 
std::unordered_map<std::string, CompDeserializeFunc>& CompDeserializeMethods();


//---- Function templates ----

template <RegistrableComponent T>
void RegisterComponentType(const std::string& typeName) {
	CompDeserializeMethods()[typeName] = &T::Deserialize;
}

template <typename T>
T JsonArrayGet(const nlohmann::json& json, size_t index, T defaultValue) {
	return (json.is_array() and json.size() > index) ? json[index].get<T>() : defaultValue;
}


//---- Free functions ----

std::unique_ptr<AbstractComponent> DeserializeComponent(Actor& owner, const std::string& name, const nlohmann::json& json);

bool IsComponentRegistered(const std::string& typeName);

} // !eng::serialization


//---- Json serializer specializations ----

template <>
struct nlohmann::adl_serializer<glm::vec2> {
	static void from_json(const json& j, glm::vec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::JsonArrayGet(j, 0, 0.f);
			vec.y = eng::JsonArrayGet(j, 1, 0.f);
		}
	}

	static void to_json(json& j, const glm::vec2& vec) {
		j = json::array({ vec.x, vec.y });
	}

	static void from_json(const ordered_json& j, glm::vec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::JsonArrayGet(j, 0, 0.f);
			vec.y = eng::JsonArrayGet(j, 1, 0.f);
		}
	}

	static void to_json(ordered_json& j, const glm::vec2& vec) {
		j = json::array({ vec.x, vec.y });
	}
};

template <>
struct nlohmann::adl_serializer<glm::ivec2> {
	static void from_json(const json& j, glm::ivec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::JsonArrayGet(j, 0, 0);
			vec.y = eng::JsonArrayGet(j, 1, 0);
		}
	}

	static void to_json(json& j, const glm::ivec2& vec) {
		j = json::array({ vec.x, vec.y });
	}

	static void from_json(const ordered_json& j, glm::ivec2& vec) {
		if (j.is_array() && j.size() >= 2) {
			vec.x = eng::JsonArrayGet(j, 0, 0);
			vec.y = eng::JsonArrayGet(j, 1, 0);
		}
	}

	static void to_json(ordered_json& j, const glm::ivec2& vec) {
		j = json::array({ vec.x, vec.y });
	}
};

template <>
struct nlohmann::adl_serializer<SDL_Color> {
	static void from_json(const json& j, SDL_Color& color) {
		if (j.is_array() && j.size() >= 2) {
			color.r = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.g = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			color.b = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.a = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(json& j, const SDL_Color& color) {
		j = json::array({ color.r, color.g, color.b, color.a });
	}

	static void from_json(const ordered_json& j, SDL_Color& color) {
		if (j.is_array() && j.size() >= 2) {
			color.r = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.g = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			color.b = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			color.a = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
		}
	}

	static void to_json(ordered_json& j, const SDL_Color& color) {
		j = json::array({ color.r, color.g, color.b, color.a });
	}
};

template <>
struct nlohmann::adl_serializer<SDL_Rect> {
	static void from_json(const json& j, SDL_Rect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.y = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			rect.w = eng::JsonArrayGet(j, 2, static_cast<Uint8>(0));
			rect.h = eng::JsonArrayGet(j, 3, static_cast<Uint8>(0));
		}
	}

	static void to_json(json& j, const SDL_Rect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

	static void from_json(const ordered_json& j, SDL_Rect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::JsonArrayGet(j, 0, static_cast<Uint8>(0));
			rect.y = eng::JsonArrayGet(j, 1, static_cast<Uint8>(0));
			rect.w = eng::JsonArrayGet(j, 2, static_cast<Uint8>(0));
			rect.h = eng::JsonArrayGet(j, 3, static_cast<Uint8>(0));
		}
	}

	static void to_json(ordered_json& j, const SDL_Rect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

};

template <>
struct nlohmann::adl_serializer<SDL_FRect> {
	static void from_json(const json& j, SDL_FRect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::JsonArrayGet(j, 0, static_cast<float>(0));
			rect.y = eng::JsonArrayGet(j, 1, static_cast<float>(0));
			rect.w = eng::JsonArrayGet(j, 2, static_cast<float>(0));
			rect.h = eng::JsonArrayGet(j, 3, static_cast<float>(0));
		}
	}

	static void to_json(json& j, const SDL_FRect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

	static void from_json(const ordered_json& j, SDL_FRect& rect) {
		if (j.is_array() && j.size() >= 2) {
			rect.x = eng::JsonArrayGet(j, 0, static_cast<float>(0));
			rect.y = eng::JsonArrayGet(j, 1, static_cast<float>(0));
			rect.w = eng::JsonArrayGet(j, 2, static_cast<float>(0));
			rect.h = eng::JsonArrayGet(j, 3, static_cast<float>(0));
		}
	}

	static void to_json(ordered_json& j, const SDL_FRect& rect) {
		j = json::array({ rect.x, rect.y, rect.w, rect.h });
	}

};