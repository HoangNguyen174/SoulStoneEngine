#ifndef REGISTERED_ALARM_EVENT_H
#define REGISTERED_ALARM_EVENT_H
#include <vector>
#include <string>

class Clock;

typedef void (*AlarmEventFuncPtr) ( const std::string& args, bool isRepeated, double timeToFire, const std::string& desc, Clock* caller );

class RegisteredAlarmEvent
{
	public:
		std::string			m_eventName;
		std::string			m_parameters;
		bool				m_isRepeated;
		double				m_timeToFireEvent;
		double				m_lastTimeFire;
		AlarmEventFuncPtr	m_callbackFunction;
		std::string			m_eventDescription;
};

#endif