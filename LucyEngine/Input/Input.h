#pragma once

#include "Services.h"
#include "EventSource.h"

#include "KeyboardState.h"
#include "GamepadState.h"

#include "SdbmHash.h"

#include "CommandInputGroup.h"

namespace eng {

class Input final : public service::IInput {
public:
	bool ProcessInput() override;

	CommandInputGroup& NewInputgroup(Actor& actor) override;

	EventSource<event::KeyUp>& UpEventSource();
	EventSource<event::KeyPressed>& PressedEventSource();
	EventSource<event::KeyDown>& DownEventSource();
private:
	KeyboardState m_KeyboardState{};
	GamepadState m_GamePadState{};

	EventSource<event::KeyUp> m_UpEventSource{};
	EventSource<event::KeyPressed> m_PressedEventSource{};
	EventSource<event::KeyDown> m_DownEventSource{};

	std::vector<std::unique_ptr<CommandInputGroup>> m_InputGroups{};
};

}