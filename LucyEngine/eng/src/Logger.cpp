#include "Logger.h"
#include "Ansicodes.h"
#include <iostream>
#include <chrono>
#include <format>

namespace eng {

//---------- Helper Function -------------------------------

static void BasicLog(const std::string& message) {
	auto now = std::chrono::system_clock::now();
	std::cout << std::format("[{:%H:%M:%S}] ", now)
		<< message
		<< std::endl;
}

//---------- Assert Logger ---------------------------------

void AssertLogger::Log(const std::string& message) {
	BasicLog(message);
}

void AssertLogger::LogError(const std::string& message) {
	auto now = std::chrono::system_clock::now();
	std::cout << ansi::RED_BOLD 
			  << std::format("[{:%H:%M:%S}] ", now)
		      << message 
			  << ansi::RESET 
			  << std::endl;
	assert(false, message);
}

//---------- Messagebox Logger -----------------------------

MessageBoxLogger::MessageBoxLogger(SDL_Window& window) :
	m_Window(&window) {}

void MessageBoxLogger::Log(const std::string& message) {
	BasicLog(message);
}

void MessageBoxLogger::LogError(const std::string& message) {
	auto now = std::chrono::system_clock::now();
	std::cout << ansi::RED_BOLD
		<< std::format("[{:%H:%M:%S}] ", now)
		<< message
		<< ansi::RESET
		<< std::endl;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Logged Error", message.c_str(), m_Window);
}

}