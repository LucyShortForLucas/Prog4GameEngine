#pragma once

#include "AbstractCommand.h"

namespace eng {

class AddScore final : public AbstractCommand {
public: //--------------- Constructor/Destructor/copy/move --------------

	AddScore(int score);
	~AddScore() = default;

	AddScore(const AddScore&) = delete;
	AddScore& operator=	(const AddScore&) = delete;

	AddScore(const AddScore&&) = delete;
	AddScore& operator=	(const AddScore&&) = delete;

public: //--------------------------- Execute ------------------------

	bool Execute(Actor& target) override;

public: //----------------------- fields -------------------------

	const int m_Score;

}; // !Kill

} // !eng