#pragma once 

#include "Services.h"

namespace eng {

/// @brief A logger that causes a debug assert when logging an error. Regular logs are printed to the console.
class AssertLogger : public service::ILogger {
public:
	void Log(const std::string& message) override;
	void LogError(const std::string& message) override;
};

/// @brief A logger that creates a message box when logging an error. Regular logs are printed to the console.
class MessageBoxLogger : public service::ILogger {
public:
	MessageBoxLogger(SDL_Window&);
	void Log(const std::string& message) override;
	void LogError(const std::string& message) override;
private:
	SDL_Window* m_Window;
};

}