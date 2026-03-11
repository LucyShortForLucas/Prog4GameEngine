#pragma once

#include <memory>

#include "../utils/utils.h"

namespace eng {

enum class GamepadKeys {
	Up,
	Down,
	Left,
	Right,
	Start,
	Back,
	Lthumb,
	Rthumb,
	Lshoulder,
	Rshoulder,
	A,
	B,
	X,
	Y,

	SIZE
};

class GamepadState final {
public: //--------------------|Constructor, rule of 5|-----------------------
	GamepadState(unsigned int playerNum = 0);
	~GamepadState();

	GamepadState(const GamepadState&) = delete;
	GamepadState& operator=	(const GamepadState&) = delete;

	GamepadState(const GamepadState&&) = delete;
	GamepadState& operator=	(const GamepadState&&) = delete;

public: //----------------|State|-------------------------------------

	void FetchCurrentState();

	bool IsKeyDown(GamepadKeys key);
	bool IsKeyPressed(GamepadKeys key);
	bool IsKeyUp(GamepadKeys key);

private: //---------------|Pimpl|------------------------------------
	class impl;
	std::unique_ptr<impl> m_ImplUptr;
};

} // !eng

