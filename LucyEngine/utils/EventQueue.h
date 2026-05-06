#pragma once

#include "EventSource.h"
#include <queue>

namespace eng {

template <typename TEvent>
class EventQueue final : public AbstractEventListener<TEvent>{
public:
	void OnEvent(const TEvent& event) override {
		m_Events.emplace_back(event);
	}

	TEvent PopEvent() {
		assert(!m_Events.empty() && "Trying to pop an empty event queue");
		auto result{ m_Events.back() };
		m_Events.pop_back();
		return result;
	}

	bool Empty() const {
		return m_Events.empty();
	}

private:
	std::vector<TEvent> m_Events{};
};

}