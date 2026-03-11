#pragma once

#include "KeyboardState.h"
#include "GamepadState.h"

namespace eng {

struct Key {
	const bool isKeyboardKey;
	union {
		SDL_Scancode keyboardKey;
		GamepadKeys gamepadKey;
	};

	Key(SDL_Scancode key) :
		isKeyboardKey(true),
		keyboardKey(key) {}

	Key(GamepadKeys key) :
		isKeyboardKey(false),
		gamepadKey(key) {}

};

}