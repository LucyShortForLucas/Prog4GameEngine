#define WIN32_LEAN_AND_MEAN

#include "GamepadState.h"
#include <windows.h>
#include <Xinput.h>
#include <bitset>
#include <iostream>
#include <format>
#include <cassert>

namespace eng {

DWORD GamepadKeyToXinputDword(GamepadKeys key) {
	switch (key) {
	case eng::GamepadKeys::Up:
		return XINPUT_GAMEPAD_DPAD_UP;
	case eng::GamepadKeys::Down:
		return XINPUT_GAMEPAD_DPAD_DOWN;
	case eng::GamepadKeys::Left:
		return XINPUT_GAMEPAD_DPAD_LEFT;
	case eng::GamepadKeys::Right:
		return XINPUT_GAMEPAD_DPAD_RIGHT;
	case eng::GamepadKeys::Start:
		return XINPUT_GAMEPAD_START;
	case eng::GamepadKeys::Back:
		return XINPUT_GAMEPAD_BACK;
	case eng::GamepadKeys::Lthumb:
		return XINPUT_GAMEPAD_LEFT_THUMB;
	case eng::GamepadKeys::Rthumb:
		return XINPUT_GAMEPAD_RIGHT_THUMB;
	case eng::GamepadKeys::Lshoulder:
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case eng::GamepadKeys::Rshoulder:
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case eng::GamepadKeys::A:
		return XINPUT_GAMEPAD_A;
	case eng::GamepadKeys::B:
		return XINPUT_GAMEPAD_B;
	case eng::GamepadKeys::X:
		return XINPUT_GAMEPAD_X;
	case eng::GamepadKeys::Y:
		return XINPUT_GAMEPAD_Y;
	default:
		assert(false and "Invalid gamepad key");
		return 0;
	}
}

class GamepadState::impl final {
public:
	GamepadState::impl(unsigned int playerNum) :
		m_PlayerNum(playerNum) {
	}

public:
	void FetchCurrentState() {
		XINPUT_STATE f_InputState;
		SecureZeroMemory(&f_InputState, sizeof(XINPUT_STATE));
		if (XInputGetState(m_PlayerNum, &f_InputState) == ERROR_DEVICE_NOT_CONNECTED) return; // Controller not attached

		if (f_InputState.dwPacketNumber == m_LastPacketNumber) return; // Data hasn't changes
		m_LastPacketNumber = f_InputState.dwPacketNumber; // Update packet number

		auto f_PrevPressedKeys{ m_PressedKeys };
		m_PressedKeys = f_InputState.Gamepad.wButtons;
		m_DownKeys = ~f_PrevPressedKeys & m_PressedKeys;
		m_UpKeys = f_PrevPressedKeys & ~m_PressedKeys;
	}

	bool IsKeyDown(GamepadKeys key) {
		return m_DownKeys & GamepadKeyToXinputDword(key);
	}
	bool IsKeyPressed(GamepadKeys key) {
		return m_PressedKeys & GamepadKeyToXinputDword(key);

	}
	bool IsKeyUp(GamepadKeys key) {
		return m_UpKeys & GamepadKeyToXinputDword(key);

	}
private: //---------------|metadata|------------

	const unsigned int m_PlayerNum;
	DWORD m_LastPacketNumber{};

private: //------------|State|---------------
	DWORD m_PressedKeys{};
	DWORD m_UpKeys{};
	DWORD m_DownKeys{};
};

GamepadState::GamepadState(unsigned int playerNum) :
	m_ImplUptr(std::make_unique<impl>(playerNum)) {

}

GamepadState::~GamepadState() = default;

void GamepadState::FetchCurrentState() {
	m_ImplUptr->FetchCurrentState();
}

bool GamepadState::IsKeyDown(GamepadKeys key) {
	return m_ImplUptr->IsKeyDown(key);
}

bool GamepadState::IsKeyPressed(GamepadKeys key) {
	return m_ImplUptr->IsKeyPressed(key);

}

bool eng::GamepadState::IsKeyUp(GamepadKeys key) {
	return m_ImplUptr->IsKeyUp(key);
}

} // !eng