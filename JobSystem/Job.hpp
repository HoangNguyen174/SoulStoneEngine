#ifndef JOB_H
#define JOB_H

class Job;

typedef void (*JobCallbackFunctionPointer)( Job* finishedJob );

enum JobPriority { JOB_LOW_PRIORITY, JOB_NORMAL_PRIORITY, JOB_HIGH_PRIORITY };

class Job
{
	public:
		JobCallbackFunctionPointer			m_callbackFunction;
		JobPriority							m_priority;

	public:
		Job() 
		{
			m_priority = JOB_NORMAL_PRIORITY;
		};
		virtual void Execute( void* args ) = 0;
		bool operator<( const Job& other ) const;
};

inline bool Job::operator<(const Job& other) const
{
	return m_priority < other.m_priority;
}

#endif;