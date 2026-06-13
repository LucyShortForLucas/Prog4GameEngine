#pragma once

#include <AbstractCommand.h>

namespace tron {

class ClearAndLoad : public eng::AbstractCommand {
public:
	//---- Ctor
	ClearAndLoad(const std::string& toLoad) : m_ToLoad{ toLoad } {}

	//---- Command methods
	bool Execute(eng::Actor& actor) override;

private:
	//---- State fields
	const std::string m_ToLoad;
};

}