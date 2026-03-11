#pragma once

#include "Utils.h"

#include <bitset>

namespace eng {

class KeyboardState final {
public: //--------------- Constructor/Destructor/copy/move --------------

	KeyboardState() = default;
	~KeyboardState() = default;

	KeyboardState(const KeyboardState&) = delete;
	KeyboardState& operator=	(const KeyboardState&) = delete;

	KeyboardState(const KeyboardState&&) = delete;
	KeyboardState& operator=	(const KeyboardState&&) = delete;

public: //-------------- State -------------------

	void FetchCurrentState();

	bool IsKeyDown(SDL_Scancode key);
	bool IsKeyPressed(SDL_Scancode key);
	bool IsKeyUp(SDL_Scancode key);


private: //--------------------------- Button state ----------------------------

	std::bitset<SDL_SCANCODE_COUNT> m_DownKeys{};
	std::bitset<SDL_SCANCODE_COUNT> m_PressedKeys{};
	std::bitset<SDL_SCANCODE_COUNT> m_UpKeys{};

}; // !KeyboardState

} // !eng