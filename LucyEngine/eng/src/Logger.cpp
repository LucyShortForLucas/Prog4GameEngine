#include "Logger.h"
#include "Ansicodes.h"
#include <iostream>
#include <chrono>
#include <format>
#include <Utils.h>

namespace eng {

//---------- Helper Function -------------------------------

static void ConsolePrint(const std::string& message, const std::string& ansiColorCode = "") {
	auto zoned = ZonedSecondsNow();
	std::cout << ansiColorCode
		<< std::format("[{:%H:%M:%S}] ", zoned)
		<< message
		<< ansi::RESET
		<< std::endl;
}

//---------- Assert Logger ---------------------------------

void AssertLogger::Log(const std::string& message) {
	ConsolePrint(message);
}

void AssertLogger::LogSuccess(const std::string& message) {
	ConsolePrint(message, ansi::GREEN_BOLD);
}

void AssertLogger::LogError(const std::string& message) {
	ConsolePrint(message, ansi::RED_BOLD);
	assert(false && message.c_str());
}

//---------- Messagebox Logger -----------------------------

MessageBoxLogger::MessageBoxLogger(SDL_Window& window) :
	m_Window(&window) {}

void MessageBoxLogger::Log(const std::string& message) {
	ConsolePrint(message);
}

void MessageBoxLogger::LogSuccess(const std::string& message) {
	ConsolePrint(message, ansi::GREEN_BOLD);
}

void MessageBoxLogger::LogError(const std::string& message) {
	ConsolePrint(message, ansi::RED_BOLD);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Logged Error", message.c_str(), m_Window);
}

}