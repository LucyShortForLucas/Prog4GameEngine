#include "Input.h"
#include "SDL3/SDL.h"
#include "backends/imgui_impl_sdl3.h"

#include "KeyboardState.h"
#include "GamepadState.h"
#include "SdbmHash.h"

#include <iostream>

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

	m_KeyboardState.FetchCurrentState();
	m_GamePadState.FetchCurrentState();

	for (int i{}; i < SDL_SCANCODE_COUNT; ++i) {
		if (m_KeyboardState.IsKeyDown(static_cast<SDL_Scancode>(i)))
			m_EventSource.Invoke(
				eventId::keyDown,
				std::make_any< eventContext::Key>(static_cast<SDL_Scancode>(i))
			);
		else if (m_KeyboardState.IsKeyUp(static_cast<SDL_Scancode>(i)))
			m_EventSource.Invoke(
				eventId::keyUp,
				std::make_any< eventContext::Key>(static_cast<SDL_Scancode>(i))
			);

		if (m_KeyboardState.IsKeyPressed(static_cast<SDL_Scancode>(i)))
			m_EventSource.Invoke(
				eventId::keyPressed,
				std::make_any< eventContext::Key>(static_cast<SDL_Scancode>(i))
			);
	}

	for (int i{}; i < static_cast<int>(GamepadKeys::SIZE); ++i) {
		if (m_GamePadState.IsKeyDown(static_cast<GamepadKeys>(i)))
			m_EventSource.Invoke(
				eventId::keyDown,
				std::make_any< eventContext::Key>(static_cast<GamepadKeys>(i))
			);
		else if (m_GamePadState.IsKeyUp(static_cast<GamepadKeys>(i)))
			m_EventSource.Invoke(
				eventId::keyUp,
				std::make_any< eventContext::Key>(static_cast<GamepadKeys>(i))
			);

		if (m_GamePadState.IsKeyPressed(static_cast<GamepadKeys>(i)))
			m_EventSource.Invoke(
				eventId::keyPressed,
				std::make_any< eventContext::Key>(static_cast<GamepadKeys>(i))
			);
	}

	return true;
}

}