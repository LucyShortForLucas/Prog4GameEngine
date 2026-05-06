#include "CommandInputGroup.h"
#include "Input.h"

namespace eng {

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

// Helper func
static void ExecuteKeyCommands(Actor& actor, Key& key, std::map<SDL_Scancode, std::unique_ptr<AbstractCommand>>& keyboardBinds, std::map<GamepadKeys, std::unique_ptr<AbstractCommand>>& gamepadBinds) {
	if (key.isKeyboardKey) {
		if (keyboardBinds.contains(key.keyboardKey) and keyboardBinds.at(key.keyboardKey))
			keyboardBinds.at(key.keyboardKey)->Execute(actor);
	}
	else {
		if (gamepadBinds.contains(key.gamepadKey) and gamepadBinds.at(key.gamepadKey))
			gamepadBinds.at(key.gamepadKey)->Execute(actor);
	}
}

void eng::CommandInputGroup::Update() {
	while (!m_DownEventQueue.Empty()) {
		auto event{ m_DownEventQueue.PopEvent() };
		auto& key{ event.key };

		ExecuteKeyCommands(*m_Actor, key, m_KeyboardBindsDown, m_GamepadBindsDown);
	}

	while (!m_PressedEventQueue.Empty()) {
		auto event{ m_PressedEventQueue.PopEvent() };
		auto& key{ event.key };

		ExecuteKeyCommands(*m_Actor, key, m_KeyboardBindsPressed, m_GamepadBindsPressed);
	}

	while (!m_UpEventQueue.Empty()) {
		auto event{ m_UpEventQueue.PopEvent() };
		auto& key{ event.key };

		ExecuteKeyCommands(*m_Actor, key, m_KeyboardBindsUp, m_GamepadBindsUp);
	}

}

void eng::CommandInputGroup::SubscribeInputSource(KeyEventSources sources) {
	sources.down.Subscribe(m_DownEventQueue);
	sources.pressed.Subscribe(m_PressedEventQueue);
	sources.up.Subscribe(m_UpEventQueue);
}

void eng::CommandInputGroup::UnsubscribeInputSource(KeyEventSources sources) {
	sources.down.Subscribe(m_DownEventQueue);
	sources.pressed.Subscribe(m_PressedEventQueue);
	sources.up.Subscribe(m_UpEventQueue);
}


} // !eng