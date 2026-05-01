#include "EventSource.h"

void eng::EventSource::Invoke(unsigned int id, std::any context) {
	for (auto listener : m_Subjects) {
		listener->OnEvent(id, context);
	}
}

void eng::EventSource::Subscribe(AbstractEventListener& subject) {
	m_Subjects.emplace(&subject);
}

void eng::EventSource::Unsubscribe(AbstractEventListener& subject) {
	m_Subjects.erase(&subject);
}