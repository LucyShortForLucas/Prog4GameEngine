#pragma once

#include "AbstractCommand.h"

namespace eng {

class Kill final : public AbstractCommand {
public: //--------------- Constructor/Destructor/copy/move --------------

	Kill() = default;
	~Kill() = default;

	Kill(const Kill&) = delete;
	Kill& operator=	(const Kill&) = delete;

	Kill(const Kill&&) = delete;
	Kill& operator=	(const Kill&&) = delete;

public: //---------------------------|Execute|------------------------
	
	bool Execute(Actor& target) override;

}; // !Kill

} // !eng