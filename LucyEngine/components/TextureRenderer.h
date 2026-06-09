#pragma once

#pragma once

#include "AbstractComponent.h"
#include "Texture2D.h"
#include <SDL3/SDL.h>
#include "Serialization.h"

namespace eng {

DECL_COMPONENT(TextureRenderer, public AbstractComponent)
public: //--------------- Constructor/Destructor/copy/move --------------

    TextureRenderer(eng::Actor& owner, const std::string& texturePath = "", glm::ivec2 size = {-1, -1}, SDL_FRect sourceRect = {-1, -1, -1, -1}, unsigned int layer = 0);
    ~TextureRenderer() override = default;

    nlohmann::ordered_json Serialize() override;

public: //------------------------ Texture Methods -------------------------

    void LoadTexture(const std::string& file);
    void SetSourceRect(SDL_FRect rect);
    void SetTexture(dae::Texture2D* texturePtr, glm::ivec2 size = { -1, -1 }, SDL_FRect sourceRect = { -1, -1, -1, -1 }, unsigned int layer = 0);

public: //------------------------ Gameloop Methods -------------------------

    void Render() override;

private: //--------------------------- Fields ----------------------------
    unsigned int m_Layer{};
    std::string m_TexturePath;
    dae::Texture2D* m_TexturePtr{};
    glm::ivec2 m_Size{};
    SDL_FRect m_SourceRect{};

}; // !TextureRenderer
REGISTER_COMPONENT(TextureRenderer)

} // !eng


/*

---- JSON TEMPLATE ----

{
    "Type": "TextureRenderer",
    "Json": {
        "TexturePath": "",
        "Size": [0,0],
        "SourceRect": [0, 0, 0, 0],
        "Layer": 0
    }
}


*/