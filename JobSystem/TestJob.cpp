#include "TestJob.hpp"

std::string TestJob::s_stringToTestJob;

TestJob::TestJob( const std::string& string )
{
	m_returnString = string;
}

void TestJob::Execute( void* args )
{
	m_returnString = "Job executes: " + m_returnString;
}

void OnTestJobComplete ( Job* finishedJob )
{
	TestJob* testJob = dynamic_cast<TestJob*>( finishedJob );

	TestJob::s_stringToTestJob = testJob->m_returnString;
}