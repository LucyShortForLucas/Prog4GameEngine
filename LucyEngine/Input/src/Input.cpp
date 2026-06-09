#include "Input.h"
#include "SDL3/SDL.h"
#include "backends/imgui_impl_sdl3.h"

#include "KeyboardState.h"
#include "GamepadState.h"
#include "SdbmHash.h"

#include <iostream>

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

namespace eng {

bool Input::ProcessInput() {
	// Poll SDL evdents
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}

		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
	#endif 

	m_KeyboardState.FetchCurrentState();
	m_GamePadState.FetchCurrentState();

	for (int i{}; i < SDL_SCANCODE_COUNT; ++i) {
		if (m_KeyboardState.IsKeyDown(static_cast<SDL_Scancode>(i)))
			m_DownEventSource.Invoke(
				event::KeyDown{ Key{ static_cast<SDL_Scancode>(i) } }
			);
		else if (m_KeyboardState.IsKeyUp(static_cast<SDL_Scancode>(i)))
			m_UpEventSource.Invoke(
				event::KeyUp{ Key{ static_cast<SDL_Scancode>(i) } }
			);

		if (m_KeyboardState.IsKeyPressed(static_cast<SDL_Scancode>(i)))
			m_PressedEventSource.Invoke(
				event::KeyPressed{ Key{ static_cast<SDL_Scancode>(i) } }
			);
	}

	for (int i{}; i < static_cast<int>(GamepadKeys::SIZE); ++i) {
		if (m_GamePadState.IsKeyDown(static_cast<GamepadKeys>(i)))
			m_DownEventSource.Invoke(
				event::KeyDown{ Key{ static_cast<GamepadKeys>(i) } }
			);
		else if (m_GamePadState.IsKeyUp(static_cast<GamepadKeys>(i)))
			m_PressedEventSource.Invoke(
				event::KeyPressed{ Key{ static_cast<GamepadKeys>(i) } }
			);

		if (m_GamePadState.IsKeyPressed(static_cast<GamepadKeys>(i)))
			m_UpEventSource.Invoke(
				event::KeyUp{ Key{ static_cast<GamepadKeys>(i) } }
			);
	}

	for (auto& inputGroup : m_InputGroups) {
		inputGroup->Update();
	}

	return true;
}

CommandInputGroup& Input::NewInputgroup(Actor& actor) {
	auto& group{ m_InputGroups.emplace_back(std::make_unique<CommandInputGroup>(&actor)) };
	group->SubscribeInputSource(CommandInputGroup::KeyEventSources{m_DownEventSource, m_PressedEventSource, m_UpEventSource});
	return *m_InputGroups.back();
}

EventSource<event::KeyUp>& Input::UpEventSource() {
	return m_UpEventSource;
}
EventSource<event::KeyPressed>& Input::PressedEventSource() {
	return m_PressedEventSource;

}
EventSource<event::KeyDown>& Input::DownEventSource() {
	return m_DownEventSource;

}

}