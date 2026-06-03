#pragma once

#include <cstdint>
#include <any>
#include <vector>
#include <set>

namespace eng {

template <typename TEvent>
class AbstractEventListener {
public:
	virtual ~AbstractEventListener() = default;
	virtual void OnEvent(const TEvent& event) = 0;
};

template <typename TEvent>
class EventSource final {
public:
	void Invoke(const TEvent& event) const {
		auto subjectsCopy{m_Subjects}; // Iterate over copy of subjects so that erasing/adding subjects during an invoke does not blow up the universe
		for (auto listener : subjectsCopy) {
			listener->OnEvent(event);
		}
	}
	void Subscribe(AbstractEventListener<TEvent>& subject) {
		auto it = std::find(m_Subjects.begin(), m_Subjects.end(), &subject);
		if (it == m_Subjects.end()) {
			m_Subjects.emplace_back(&subject);
		}
	}
	void Unsubscribe(AbstractEventListener<TEvent>& subject) {
		auto it = std::find(m_Subjects.begin(), m_Subjects.end(), &subject);
		if (it != m_Subjects.end()) {
			*it = m_Subjects.back();
			m_Subjects.pop_back();
		}
	}

private:
	std::vector<AbstractEventListener<TEvent>*> m_Subjects;
};

/// @brief A helper macro to implement a private EventSource member in a class and automatically generate the appropriate public Subscribe/Unsubcsribe methods.
/// for an event x named Foo, creates a member 'm_FooEvent' and the methods 'SubscribeFoo(AbstractEventListener<event::Foo>& subject)' and 'SubscribeFoo(AbstractEventListener<event::Foo>& subject)'
/// All events must be within the event namespace at the class' namespace scope.
/// Leaves the class body open at private access.
#define DECL_EVENT(x) public: \
void Subscribe##x(AbstractEventListener<event::x>& subject) { m_##x##Event.Subscribe(subject); } \
void Unsubscribe##x(AbstractEventListener<event::x>& subject) { m_##x##Event.Unsubscribe(subject); } \
private: \
EventSource<event::x> m_##x##Event{};

}