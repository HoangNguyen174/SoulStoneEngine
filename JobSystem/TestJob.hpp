#ifndef TEST_JOB
#define TEST_JOB
#include "Job.hpp"
#include <string>

class TestJob : public Job
{
	public:
		static std::string  s_stringToTestJob;
		std::string			m_returnString;

	public:
		TestJob( const std::string& tring );
		void Execute( void* args );
};

void OnTestJobComplete ( Job* finishedJob );

#endif