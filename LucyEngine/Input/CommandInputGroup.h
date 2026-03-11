#pragma once

#include <map>
#include <SDL3/SDL.h>
#include "AbstractCommand.h"
#include "EventQueue.h"
#include "Key.h"

namespace eng {

class CommandInputGroup final {
public:
	CommandInputGroup(Actor* m_Actor = nullptr);

	void SetActor(Actor* m_Actor);

	void SubscribeKeyDown(Key key, std::unique_ptr<AbstractCommand> command);
	void SubscribeKeyPressed(Key key, std::unique_ptr<AbstractCommand> command);
	void SubscribeKeyUp(Key key, std::unique_ptr<AbstractCommand> command);

	void UnsubscribeKeyDown(Key key);
	void UnsubscribeKeyPressed(Key key);
	void UnsubscribeKeyUp(Key key);

	void UnsubscribeAll(Key key);

	void Update();

	void SubscribeInputSource(EventSource& source);
	void UnsubscribeInputSource(EventSource& source);

private:
	Actor* m_Actor;

	std::map<SDL_Scancode, std::unique_ptr<AbstractCommand>> m_KeyboardBindsDown{};
	std::map<SDL_Scancode, std::unique_ptr<AbstractCommand>> m_KeyboardBindsPressed{};
	std::map<SDL_Scancode, std::unique_ptr<AbstractCommand>> m_KeyboardBindsUp{};

	std::map<GamepadKeys, std::unique_ptr<AbstractCommand>> m_GamepadBindsDown{};
	std::map<GamepadKeys, std::unique_ptr<AbstractCommand>> m_GamepadBindsPressed{};
	std::map<GamepadKeys, std::unique_ptr<AbstractCommand>> m_GamepadBindsUp{};

	EventQueue m_EventQueue{};
};

}