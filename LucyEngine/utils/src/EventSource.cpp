#include "EventSource.h"

void eng::EventSource::Invoke(unsigned int id, std::any context) {
	for (auto listener : m_Subjects) {
		listener->OnEvent(id, context);
	}
}

void eng::EventSource::Subsribe(AbstractEventListener& subject) {
	m_Subjects.emplace(&subject);
}

void eng::EventSource::UnSubsribe(AbstractEventListener& subject) {
	m_Subjects.erase(&subject);
}