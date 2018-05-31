#include "JobManager.hpp"
#include "MemoryPoolManager.hpp"

std::priority_queue<Job*>		JobManager::s_pendingJobList;
std::priority_queue<Job*>		JobManager::s_completedJobList;
std::vector<Thread*>			JobManager::s_workerThreadsList;
Mutex							JobManager::s_mutexForPendingJobList;
Mutex							JobManager::s_mutexForCompleteJobList;
bool							JobManager::s_isJobManagerRunning = true;
char*							g_isJobManagerRunning = "a";

void ExecuteJob( void* /*args*/ )
{
	while( g_isJobManagerRunning /*JobManager::s_isJobManagerRunning*/ )
	{
		Job* job = JobManager::GetJobs();

		if( job != nullptr )
		{
			job->Execute( nullptr );
			JobManager::AddJobToCompleteList( job );
		}
		else
			Sleep(1);
	}
}

void JobManager::Initalize()
{
	for( int i = 0; i < MAX_THREAD_WORKER; i++ )
	{
		Thread* workerThread = new Thread( ExecuteJob );
		s_workerThreadsList.push_back( workerThread );
		workerThread->Start();
	}
}

void JobManager::AddJob( Job* job )
{
	s_mutexForPendingJobList.Lock();
	s_pendingJobList.push( job );
	s_mutexForPendingJobList.Unlock();
}

void JobManager::AddJobToCompleteList( Job* job )
{
	s_mutexForCompleteJobList.Lock();
	s_completedJobList.push( job );
	s_mutexForCompleteJobList.Unlock();
}

Job* JobManager::GetJobs()
{
	s_mutexForPendingJobList.Lock();
	Job* returnJob = nullptr;
	if( s_pendingJobList.size() != 0 )
	{
		returnJob = s_pendingJobList.top();
		s_pendingJobList.pop();
	}
	s_mutexForPendingJobList.Unlock();

	return returnJob;
}

void JobManager::Update()
{
	Job* completedJob = nullptr;
	while( s_completedJobList.size() != 0 )
	{
		s_mutexForCompleteJobList.Lock();
		completedJob = s_completedJobList.top();
		s_mutexForCompleteJobList.Unlock();

		if( completedJob->m_callbackFunction != nullptr )
			completedJob->m_callbackFunction( completedJob );

		s_mutexForCompleteJobList.Lock();
		s_completedJobList.pop();
		s_mutexForCompleteJobList.Unlock();
	}

}

void JobManager::ExitJobManager()
{
	s_isJobManagerRunning = false;
}


