#pragma once

#include "Services.h"
#include "EventSource.h"

#include "KeyboardState.h"
#include "GamepadState.h"

#include "SdbmHash.h"

#include "CommandInputGroup.h"

namespace eng {

namespace eventId {

constexpr unsigned int keyDown{ make_sdbm_hash("KeyDown") };
constexpr unsigned int keyPressed{ make_sdbm_hash("KeyPressed") };
constexpr unsigned int keyUp{ make_sdbm_hash("KeyUp") };

}

class Input final : public service::IInput {
public:
	bool ProcessInput() override;

	CommandInputGroup& NewInputgroup(Actor& actor) override;

	EventSource& GetEventSource();

private:
	KeyboardState m_KeyboardState{};
	GamepadState m_GamePadState{};

	EventSource m_EventSource{};

	std::vector<std::unique_ptr<CommandInputGroup>> m_InputGroups{};
};

}