#pragma once 

#include "Services.h"

namespace eng {

class AssertLogger : public service::ILogger {									/// A logger that causes a debug assert when logging an error. Regular logs are printed to the console.
public:
	void Log(const std::string& message) override;
	void LogSuccess(const std::string& message) override;
	void LogError(const std::string& message) override;
};

class MessageBoxLogger : public service::ILogger {								/// A logger that creates a message box when logging an error. Regular logs are printed to the console.
public:
	MessageBoxLogger(SDL_Window&);
	void Log(const std::string& message) override;
	void LogSuccess(const std::string& message) override;
	void LogError(const std::string& message) override;
private:
	SDL_Window* m_Window;
};

}