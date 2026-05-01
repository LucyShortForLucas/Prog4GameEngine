#pragma once

#include <cstdint>
#include <any>
#include <vector>
#include <set>

namespace eng {

//class AbstractEventListener {
//public:
//	virtual ~AbstractEventListener() = default;
//	virtual void OnEvent(unsigned int id, std::any context) = 0;
//};

//class EventSource final {
//public:
//	void Invoke(unsigned int id, std::any context);
//	void Subscribe(AbstractEventListener& subject);
//	void Unsubscribe(AbstractEventListener& subject);
//
//private:
//	std::set<AbstractEventListener*> m_Subjects;
//};

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

}