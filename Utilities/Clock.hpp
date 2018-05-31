#ifndef CLOCK_H
#define CLOCK_H
#undef new
#include <vector>
#include <map>
//#define new new( __FILE__, __LINE__ )

#include "RegisteredAlarmEvent.hpp"
#include "Time.hpp"

class Clock
{
	private:
		Clock();

	public:
		static Clock									s_masterClock;
		std::vector<Clock*>								m_childClockList;
		std::map<std::string, RegisteredAlarmEvent*>	m_registeredAlarmEventList;
		double											m_currentTime;
		double											m_lastDeltaSecond;
		double											m_timeScale;
		double											m_maximumDelta;
		bool											m_isPaused;
		Clock*											m_parent;

	public:
		Clock( Clock& parent );
		void RemoveChildClock( Clock* childClock );
		void AdvanceTime( double deltaSecond );
		bool FireAlarmEvent( const std::string& eventName );
		void FireAlarmEvents();
		void Pause();
		void Unpause();
		void AddAlarmEvent( const std::string& eventName, double timeToFire, const std::string& parameters, bool isRepeated, AlarmEventFuncPtr funcPtr, const std::string& desc );
		float GetPercentElapsed( const std::string& eventName );
		double GetSecondsElapsed( const std::string& eventName );
};

void TestAlarmEvent( const std::string& args, bool isRepeated, double timeToFire, const std::string& eventName, Clock* caller  );

#endif