#include "ProfileSection.hpp"
#include "GameCommon.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

std::map<std::string, ProfileSection*>	g_registeredProfileSectionList;
ProfileSection ProfileSection::s_mainLoopProfileSection;

ProfileSection::ProfileSection()
{
	m_name = "Main Loop";
	m_category = MAIN_LOOP;
	m_startTimeSec = 0.0;
	m_endTimeSec = 0.0;
	m_secSpentPerFrame = 0.0;
	m_averageSecSpentPerFrame = 0.0;
	m_totalTime = 0.0;
	m_count = 0;
	g_registeredProfileSectionList[ m_name ] = this;
}

ProfileSection::ProfileSection(const std::string& name, PROFILE_CATEGORY category)
{
	m_name = name;
	m_category = category;
	m_startTimeSec = 0.0;
	m_endTimeSec = 0.0;
	m_secSpentPerFrame = 0.0;
	m_averageSecSpentPerFrame = 0.0;
	m_totalTime = 0.0;
	m_count = 0;

	std::map<std::string, ProfileSection*>::iterator iter;
	for( iter = g_registeredProfileSectionList.begin(); iter != g_registeredProfileSectionList.end(); ++iter )
	{
		std::string profileName = iter->first;
		if( profileName.compare( name ) == 0 )
		{
			DebuggerPrintf( "Profile with name %s already exist! Exit Program.\n", name );
			exit(0);
		}
	}

	g_registeredProfileSectionList[ name ] = this;
}

void ProfileSection::StartProfileSection()
{
	m_startTimeSec = GetCurrentTimeSeconds();
}

void ProfileSection::EndProfileSection()
{
	m_endTimeSec = GetCurrentTimeSeconds();
	m_secSpentPerFrame = m_endTimeSec - m_startTimeSec;
	m_totalTime += m_secSpentPerFrame;
	m_count++;
	m_averageSecSpentPerFrame = m_totalTime / static_cast<double>( m_count );
}

void ReportProfilingResultCommand( const std::string& args )
{
	std::string report;
	std::string numStr;
	double mainLoopTotalTime = 0.0;
	Command temp;

	ProfileSection* mainLoop = nullptr;

	std::map<std::string, ProfileSection*>::iterator iter;
	for( iter = g_registeredProfileSectionList.begin(); iter != g_registeredProfileSectionList.end(); ++iter )
	{
		std::string name = iter->first;
		if( name.compare( "Main Loop" ) == 0 )
		{
			mainLoop = iter->second;
			break;
		}
	}

	mainLoopTotalTime = mainLoop->m_totalTime;

	//display main loop
	report.append( " Main Loop: ");
	report.append( " Total Time Spent: " );
	numStr = std::to_string( static_cast<long double>( mainLoop->m_totalTime ) );
	report.append( numStr + " sec. " );
	report.append( " Number of run: ");
	numStr = std::to_string( static_cast<long double>( mainLoop->m_count ) );
	report.append( numStr + ". " );
	report.append( " Average Time Spent: ");
	numStr = std::to_string( static_cast<long double>( mainLoop->m_averageSecSpentPerFrame ) );
	report.append( numStr + " sec. " );

	temp.color = RGBColor::White();
	temp.message = report;
	g_theConsole->m_historyCommandListToPrint.push_back( temp );

	//display sub section
	for( iter = g_registeredProfileSectionList.begin(); iter != g_registeredProfileSectionList.end(); ++iter )
	{
		ProfileSection* profileSection = iter->second;
		if( profileSection->m_name.compare( "Main Loop" ) == 0 )
			continue;

		Command temp2;
		report.clear();

		report.append( " " + profileSection->m_name + ": " );
		report.append( " Total Time Spent: " );
		numStr = std::to_string( static_cast<long double>( profileSection->m_totalTime ) );
		report.append( numStr + " sec. " );
		report.append( " Number of run: ");
		numStr = std::to_string( static_cast<long double>( profileSection->m_count ) );
		report.append( numStr + ". " );
		report.append( " Average Time Spent: ");
		numStr = std::to_string( static_cast<long double>( profileSection->m_averageSecSpentPerFrame ) );
		report.append( numStr + " sec. " );
		float percentOverMainLoop = 0.f;
		percentOverMainLoop = profileSection->m_totalTime / mainLoopTotalTime;
		report.append( " Percent Time Spent Over Main Loop: ");
		numStr = std::to_string( static_cast<long double>( percentOverMainLoop ) );
		report.append( numStr + "%. " );

		temp2.color = RGBColor::White();
		temp2.message = report;
		g_theConsole->m_historyCommandListToPrint.push_back( temp2 );
	}

}

