#include "EventSource.h"
#include <queue>

namespace eng {

class EventQueue final : public AbstractEventListener {
public:
	using Event = std::pair<unsigned int, std::any>;

	void OnEvent(unsigned int id, std::any context) override;

	Event PopEvent();

private:
	std::queue<Event> m_Events{};
};

}