#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

#undef new
#include <vector>
#include <queue>
//#define new new( __FILE__, __LINE__ )

#include "Thread.hpp"
#include "Job.hpp"

const int MAX_THREAD_WORKER = 8;

class JobManager
{
	public:
		static std::priority_queue<Job*>		s_pendingJobList;
		static std::priority_queue<Job*>		s_completedJobList;
		static std::vector<Thread*>				s_workerThreadsList;
		static Mutex							s_mutexForPendingJobList;
		static Mutex							s_mutexForCompleteJobList;
		static bool								s_isJobManagerRunning;

	public:
		static void Initalize();
		static void Update();
		static void AddJob( Job* job );
		static void AddJobToCompleteList( Job* job );
		static void RemoveJobFromCompleteList( Job* job );
		static Job* GetJobs();
		static void CloseAllWorkerThread();
		static void ExitJobManager();
};

extern char* g_isJobManagerRunning;
#endif