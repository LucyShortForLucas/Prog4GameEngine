#include "CommandInputGroup.h"
#include "Input.h"

eng::CommandInputGroup::CommandInputGroup(Actor* actor) :
	m_Actor(actor) {
}

void eng::CommandInputGroup::SetActor(Actor* actor) {
	m_Actor = actor;
}

void eng::CommandInputGroup::SubscribeKeyDown(Key key, std::unique_ptr<AbstractCommand> command) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsDown.emplace(key.keyboardKey, std::move(command));
	}
	else {
		m_GamepadBindsDown.emplace(key.gamepadKey, std::move(command));
	}
}

void eng::CommandInputGroup::SubscribeKeyPressed(Key key, std::unique_ptr<AbstractCommand> command) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsPressed.emplace(key.keyboardKey, std::move(command));
	}
	else {
		m_GamepadBindsPressed.emplace(key.gamepadKey, std::move(command));
	}
}

void eng::CommandInputGroup::SubscribeKeyUp(Key key, std::unique_ptr<AbstractCommand> command) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsUp.emplace(key.keyboardKey, std::move(command));
	}
	else {
		m_GamepadBindsUp.emplace(key.gamepadKey, std::move(command));
	}
}

void eng::CommandInputGroup::UnsubscribeKeyDown(Key key) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsDown.erase(key.keyboardKey);
	}
	else {
		m_GamepadBindsDown.erase(key.gamepadKey);
	}
}

void eng::CommandInputGroup::UnsubscribeKeyPressed(Key key) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsPressed.erase(key.keyboardKey);
	}
	else {
		m_GamepadBindsPressed.erase(key.gamepadKey);
	}
}

void eng::CommandInputGroup::UnsubscribeKeyUp(Key key) {
	if (key.isKeyboardKey) {
		m_KeyboardBindsUp.erase(key.keyboardKey);
	}
	else {
		m_GamepadBindsUp.erase(key.gamepadKey);
	}
}

void eng::CommandInputGroup::UnsubscribeAll(Key key) {
	UnsubscribeKeyDown(key);
	UnsubscribeKeyPressed(key);
	UnsubscribeKeyUp(key);
}

void eng::CommandInputGroup::Update() {
	while (not m_EventQueue.Empty()) {
		auto event{ m_EventQueue.PopEvent() };
		switch (event.first) {

		case eventId::keyDown: {
			auto key{ std::any_cast<Key>(event.second) };

			if (key.isKeyboardKey) {
				if (m_KeyboardBindsDown.contains(key.keyboardKey) and m_KeyboardBindsDown.at(key.keyboardKey))
					m_KeyboardBindsDown.at(key.keyboardKey)->Execute(*m_Actor);
			}
			else {
				if (m_GamepadBindsDown.contains(key.gamepadKey) and m_GamepadBindsDown.at(key.gamepadKey))
					m_GamepadBindsDown.at(key.gamepadKey)->Execute(*m_Actor);
			}

			break;
		}

		case eventId::keyPressed: {
			auto key{ std::any_cast<Key>(event.second) };

			if (key.isKeyboardKey) {
				if (m_KeyboardBindsPressed.contains(key.keyboardKey) and m_KeyboardBindsPressed.at(key.keyboardKey))
					m_KeyboardBindsPressed.at(key.keyboardKey)->Execute(*m_Actor);
			}
			else {
				if (m_GamepadBindsPressed.contains(key.gamepadKey) and m_GamepadBindsPressed.at(key.gamepadKey))
					m_GamepadBindsPressed.at(key.gamepadKey)->Execute(*m_Actor);
			}

			break;
		}

		case eventId::keyUp: {
			auto key{ std::any_cast<Key>(event.second) };

			if (key.isKeyboardKey) {
				if (m_KeyboardBindsUp.contains(key.keyboardKey) and m_KeyboardBindsUp.at(key.keyboardKey))
					m_KeyboardBindsUp.at(key.keyboardKey)->Execute(*m_Actor);
			}
			else {
				if (m_GamepadBindsUp.contains(key.gamepadKey) and m_GamepadBindsUp.at(key.gamepadKey))
					m_GamepadBindsUp.at(key.gamepadKey)->Execute(*m_Actor);
			}

			break;
		}
		}
	}
}

void eng::CommandInputGroup::SubscribeInputSource(EventSource& source) {
	source.Subsribe(m_EventQueue);
}

void eng::CommandInputGroup::UnsubscribeInputSource(EventSource& source) {
	source.UnSubsribe(m_EventQueue);
}