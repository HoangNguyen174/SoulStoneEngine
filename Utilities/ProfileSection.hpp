#ifndef PROFILE_SECTION_H
#define PROFILE_SECTION_H
#include "Clock.hpp"
#include <string>

#undef new
#include <map>
//#define new new( __FILE__, __LINE__ )

enum PROFILE_CATEGORY { MAIN_LOOP, TEST };

class ProfileSection
{
	public:
		static ProfileSection							s_mainLoopProfileSection;

	public:
		std::string										m_name;
		double											m_secSpentPerFrame;
		double											m_averageSecSpentPerFrame;
		double											m_startTimeSec;
		double											m_endTimeSec;
		double											m_totalTime;
		PROFILE_CATEGORY								m_category;
		unsigned int									m_count;

	public:
		ProfileSection( const std::string& name, PROFILE_CATEGORY category );
		void StartProfileSection();
		void EndProfileSection();

	private:
		ProfileSection();
		//static void ResetProfileSection();
};

void ReportProfilingResultCommand( const std::string& args );
void DisplayProfileOnScreen();

#endif