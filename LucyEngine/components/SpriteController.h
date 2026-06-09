#pragma once

#include "Serialization.h"
#include "Actor.h"
#include "SceneTree.h"
#include "TextureRenderer.h"

namespace eng {

DECL_COMPONENT(SpriteController, public AbstractComponent)
public:
	//---- Ctor
	SpriteController(Actor& owner, std::map<HashedString, std::pair<std::string, glm::vec2>> sprites, glm::vec2 size)
		: AbstractComponent(owner), m_Sprites(sprites), m_Size{size} {};

	//---- Serialization
	nlohmann::ordered_json Serialize() override;

	//---- Gameloop methods
	void Start() override;

	//---- SpriteController methods

	void SetSprite(HashedString sprite);

private:
	TextureRenderer* m_TextureRenderer{};
	std::map<HashedString, std::pair<std::string, glm::vec2>> m_Sprites{};
	glm::vec2 m_Size{};
};
REGISTER_COMPONENT(SpriteController);
}


/*

---- Json Template ----

{
	"Type": "SpriteController",
	"Json": {
		"Size": [0, 0],
		"Sprites": [
			{
				"Texture": "",
				"Position": [0,0]
			},
			{
				"Texture": "",
				"Position": [0,0]
			}
		]
	}
}

*/