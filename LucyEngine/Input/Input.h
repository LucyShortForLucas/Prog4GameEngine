#include "Services.h"
#include "EventSource.h"

#include "KeyboardState.h"
#include "GamepadState.h"

#include "SdbmHash.h"

namespace eng {

namespace eventId {

constexpr unsigned int keyDown{ make_sdbm_hash("KeyDown") };
constexpr unsigned int keyPressed{ make_sdbm_hash("KeyPressed") };
constexpr unsigned int keyUp{ make_sdbm_hash("KeyUp") };

}

namespace eventContext {

struct Key {
	const bool isKeyboardKey;
	union {
		SDL_Scancode keyboardKey;
		GamepadKeys gamepadKey;
	};

	Key(SDL_Scancode key) :
		isKeyboardKey(true),
		keyboardKey(key) {
	}

	Key(GamepadKeys key) :
		isKeyboardKey(false),
		gamepadKey(key) {}

};

}

class Input final : public service::IInput {
public:
	bool ProcessInput() override;

private:
	KeyboardState m_KeyboardState{};
	GamepadState m_GamePadState{};

	EventSource m_EventSource{};
};

}