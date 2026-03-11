#include <cstdint>
#include <any>
#include <vector>
#include <set>

namespace eng {

class AbstractEventListener {
public:
	virtual void OnEvent(unsigned int id, std::any context) = 0;
};

class EventSource final {
public:
	void Invoke(unsigned int id, std::any context);
	void Subsribe(AbstractEventListener& subject);
	void UnSubsribe(AbstractEventListener& subject);

private:
	std::set<AbstractEventListener*> m_Subjects;
};

}