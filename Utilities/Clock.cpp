#include "Clock.hpp"
#include "GameCommon.hpp"

Clock Clock::s_masterClock;

Clock::Clock()
{
	m_currentTime = 0.0;
	m_timeScale = 1.f;
	m_maximumDelta = FLT_MAX;
	m_isPaused = false;
	m_parent = nullptr;
	m_lastDeltaSecond = 0.0;
}

Clock::Clock( Clock& parent )
{
	m_parent = &parent;
	m_currentTime = 0.0;
	m_timeScale = 1.f;
	m_maximumDelta = m_parent->m_maximumDelta;
	m_isPaused = false;
	m_lastDeltaSecond = 0.0;
	parent.m_childClockList.push_back( this );
}

void Clock::AdvanceTime( double deltaSecond )
{
	if( deltaSecond > m_maximumDelta )
		deltaSecond = m_maximumDelta;
	if( m_isPaused )
		deltaSecond = 0.0;
	deltaSecond *= m_timeScale;
	m_lastDeltaSecond = deltaSecond;
	m_currentTime += deltaSecond;
	FireAlarmEvents();

	for( unsigned int childIndex = 0; childIndex < m_childClockList.size(); childIndex++ )
	{
		Clock* child = m_childClockList[childIndex];
		child->AdvanceTime( deltaSecond );
	}
}

void Clock::RemoveChildClock( Clock* childClock )
{
	for( unsigned int childIndex = 0; childIndex < m_childClockList.size(); childIndex++ )
	{
		Clock* target = m_childClockList[childIndex];
		if( target == childClock )
		{
			delete target;
			target = nullptr;
			m_parent->m_childClockList.erase( m_parent->m_childClockList.begin() + childIndex );
		}
	}
}

void Clock::AddAlarmEvent( const std::string& eventName, double timeToFire, const std::string& parameters, bool isRepeated, AlarmEventFuncPtr funcPtr, const std::string& desc )
{
	RegisteredAlarmEvent* registeredEvent = new RegisteredAlarmEvent();
	registeredEvent->m_callbackFunction = funcPtr;
	registeredEvent->m_eventName = eventName;
	registeredEvent->m_timeToFireEvent = timeToFire;
	registeredEvent->m_isRepeated = isRepeated;
	registeredEvent->m_parameters = parameters;
	registeredEvent->m_eventDescription = desc;
	registeredEvent->m_lastTimeFire = m_currentTime;
	m_registeredAlarmEventList[ eventName ] = registeredEvent;
}

void Clock::FireAlarmEvents()
{
	std::map<std::string, RegisteredAlarmEvent*>::iterator iter = m_registeredAlarmEventList.begin();
	while( iter != m_registeredAlarmEventList.end() )
	{
		double deltaTime = m_currentTime - iter->second->m_lastTimeFire;
		if( abs( deltaTime - iter->second->m_timeToFireEvent ) < 0.01 )
		{
			FireAlarmEvent( iter->first );
			iter = m_registeredAlarmEventList.begin();
			iter->second->m_lastTimeFire = m_currentTime;
		}
		++iter;
	}
}

bool Clock::FireAlarmEvent( const std::string& eventName )
{
	std::map<std::string, RegisteredAlarmEvent*>::iterator iter;
	iter = m_registeredAlarmEventList.find( eventName );

	if( iter == m_registeredAlarmEventList.end() )
	{
		DebuggerPrintf("Cannot find alarm event %s!\n", eventName.c_str() );
		return false;
	}
	else
	{
		iter->second->m_callbackFunction( iter->second->m_parameters, iter->second->m_isRepeated, iter->second->m_timeToFireEvent, iter->second->m_eventName, this );
		return true;
	}
}

void Clock::Pause()
{
	m_isPaused = true;
}

void Clock::Unpause()
{
	m_isPaused = false;
}

float Clock::GetPercentElapsed(const std::string& eventName)
{
	std::map<std::string, RegisteredAlarmEvent*>::iterator iter;
	iter = m_registeredAlarmEventList.find( eventName );

	if( iter != m_registeredAlarmEventList.end() )
	{
		if( iter->second->m_timeToFireEvent == 0.0 )
			return -1.0;
		return ( m_currentTime - iter->second->m_lastTimeFire ) / iter->second->m_timeToFireEvent;
	}
	else
	{
		DebuggerPrintf( "Cannot find event with name %s. Terminate Program!\n", eventName.c_str() );
		exit(0);
	}
}

double Clock::GetSecondsElapsed(const std::string& eventName)
{
	std::map<std::string, RegisteredAlarmEvent*>::iterator iter;
	iter = m_registeredAlarmEventList.find( eventName );

	if( iter != m_registeredAlarmEventList.end() )
	{
		return m_currentTime - iter->second->m_lastTimeFire;
	}
	else
	{
		DebuggerPrintf( "Cannot find event with name %s. Terminate Program!\n", eventName.c_str() );
		exit(0);
	}
}

void TestAlarmEvent( const std::string& args, bool isRepeated, double timeToFire, const std::string& eventName, Clock* caller )
{
	Command temp;
	std::string message;

	message.append( "Event \"" );
	message.append( eventName + "\" is fired.");

	temp.color = RGBColor::White();
	temp.message = message;
	g_theConsole->m_historyCommandListToPrint.push_back( temp );

	caller->m_registeredAlarmEventList.erase( eventName );

	if( isRepeated )
	{
		caller->AddAlarmEvent( eventName, timeToFire, args, isRepeated, TestAlarmEvent, "Event Description" );
	}
}