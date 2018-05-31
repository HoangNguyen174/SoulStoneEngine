#ifndef THREAD_H
#define THREAD_H
#include <Windows.h>
#include <process.h>
#include "Mutex.hpp"
#include "Job.hpp"

enum ThreadPriority { IDLE_THREAD, LOWEST_PRIORITY, LOW_PRIORITY, NORMAL_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY, CRITICAL_PRIORITY };
enum ThreadState { INITIALIZED, RUNNING, TERMINATED };

typedef unsigned int ThreadID;
static DWORD						 s_selfTlsIndex;
typedef void ( Job::*JobFunctionPointer ) ( void* args );

class Thread
{
	private:
		static Thread				s_mainThread;

	public:
		ThreadID					m_ID;
		HANDLE						m_handle;
		ThreadPriority				m_priority;
		ThreadState					m_state;
		bool						m_isDetached;
		void*						m_arguments;
		Mutex						m_mutex;
		void*						m_returnVal;
		void*						(*m_funcReturnVal)( void* );
		void						(*m_funcVoid)( void* );

	private:
		Thread();
		int TranslateToWindowPriority( ThreadPriority priority );

	public:
		Thread( void ( *function ) ( void* ), void* arg = nullptr, ThreadPriority priority = NORMAL_PRIORITY);
		Thread( void* ( *function ) ( void* ), void* arg = nullptr, ThreadPriority priority = NORMAL_PRIORITY );
		~Thread();
		void Start();
		void End();
		void Join( void** );
		static void ThreadYield();
		static void ThreadSleep( unsigned long milliseconds );
		static void Exit( void* returnVal );
		static Thread* Self();
		static int GetCurrentThreadID();
		void SetSelfStorage();

};



#endif