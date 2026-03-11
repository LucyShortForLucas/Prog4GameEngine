#include "EventQueue.h"

namespace eng {

void EventQueue::OnEvent(unsigned int id, std::any context) {
	m_Events.emplace(std::make_pair(id, context));
}

EventQueue::Event EventQueue::PopEvent() {
	auto result{m_Events.back()};
	m_Events.pop();
	return result;
}

bool eng::EventQueue::Empty() {
	return m_Events.empty();
}

}