#include "KeyboardState.h"
#include "SDL3/SDL.h"
#include <cassert>

namespace eng {

void KeyboardState::FetchCurrentState() {
	int f_SdlKeyboardSize;
	const bool* f_SdlKeyboardState{ SDL_GetKeyboardState(&f_SdlKeyboardSize) };

	// Save previous pressed key state
	auto f_PrevPressedKeys{ m_PressedKeys };

	// Set current pressed key state
	for (int i{}; i < SDL_SCANCODE_COUNT; ++i) {

		if (f_SdlKeyboardState[i] and not m_PressedKeys.test(i)) m_PressedKeys.set(i);
		else if (not f_SdlKeyboardState[i] and m_PressedKeys.test(i)) m_PressedKeys.reset(i);
	}

	//Calculate downed/released keys this frame

	m_DownKeys = ~f_PrevPressedKeys & m_PressedKeys;
	m_UpKeys = f_PrevPressedKeys & ~m_PressedKeys;
}

bool KeyboardState::IsKeyDown(SDL_Scancode key) {
	return m_DownKeys.test(static_cast<int>(key));
}

bool KeyboardState::IsKeyUp(SDL_Scancode key) {
	return m_UpKeys.test(static_cast<int>(key));
}

bool KeyboardState::IsKeyPressed(SDL_Scancode key) {
	return m_PressedKeys.test(static_cast<int>(key));
}

} // !eng::input