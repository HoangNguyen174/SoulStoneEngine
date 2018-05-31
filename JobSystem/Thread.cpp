#include "Thread.hpp"
#include "../Utilities/GameCommon.hpp"
#include "MemoryPoolManager.hpp"

extern "C"
unsigned int __stdcall ThreadWrapper( void* ptr )
{
	Thread* thread = reinterpret_cast<Thread*>( ptr );

	if( !TlsSetValue( s_selfTlsIndex, ptr ) )
	{
		DebuggerPrintf( "TlsSetValue Error. Exit Program.\n" );
		return 1;
	}
	
	if( thread->m_funcVoid != nullptr )
	{
		( *thread->m_funcVoid )( thread->m_arguments );
		Thread::Exit( nullptr );
	}

	if( thread->m_funcReturnVal != nullptr )
	{
		void* returnValue = ( *thread->m_funcReturnVal )( thread->m_arguments );
		Thread::Exit( returnValue );
	}

	return 0;
}

Thread::Thread(  void* ( *function ) ( void* ), void* arg, ThreadPriority priority )
{
	m_priority = priority;
	m_isDetached = false;
	m_arguments = arg;
	m_funcReturnVal = function;
	m_funcVoid = nullptr;
	SetSelfStorage();
}

Thread::Thread(  void ( *function ) ( void* ), void* arg, ThreadPriority priority )
{
	m_priority = priority;
	m_isDetached = true;
	m_arguments = arg;
	m_funcReturnVal = nullptr;
	m_funcVoid = function;
	SetSelfStorage();
}

Thread::Thread()
{
	m_priority = NORMAL_PRIORITY;
	m_isDetached = false;
	m_arguments = nullptr;
	m_funcReturnVal = nullptr;
	m_funcVoid = nullptr;
	SetSelfStorage();
}

Thread::~Thread()
{
	//CloseHandle( m_handle );
}

void Thread::Start()
{
	m_mutex.Lock();
	m_handle = ( HANDLE )_beginthreadex( nullptr, 0, ThreadWrapper, (LPVOID)this, CREATE_SUSPENDED, &m_ID );
	SetThreadPriority( m_handle, TranslateToWindowPriority(m_priority) );
	ResumeThread( m_handle );
	m_state = RUNNING;
	m_mutex.Unlock();
}

Thread* Thread::Self()
{
	void* t;
	t = TlsGetValue( s_selfTlsIndex );
	return reinterpret_cast<Thread*>( t );
}

void Thread::ThreadSleep( unsigned long milliseconds )
{
	Sleep( milliseconds );
}

void Thread::ThreadYield()
{
	Sleep(0);
}

void Thread::Exit( void* returnVal )
{
	Thread* thread = Self();

	if( thread )
	{
		thread->m_mutex.Lock();
		thread->m_state = TERMINATED;
		thread->m_mutex.Unlock();
	}

	if( thread->m_isDetached )
		delete thread;
	else
		thread->m_returnVal = returnVal;

	_endthreadex(0);
}

void Thread::Join(void** returnVal)
{
	m_mutex.Lock();
	if( m_state ==  RUNNING || m_state == TERMINATED )
	{
		m_mutex.Unlock();
		if( WaitForSingleObject( m_handle, INFINITE ) != WAIT_OBJECT_0 )
		{
			DebuggerPrintf( "Error Occurred While Yielding." );
			return;
		}
	}
	else
		m_mutex.Unlock();

	*returnVal = m_returnVal;

	delete this;
}

int Thread::TranslateToWindowPriority(ThreadPriority priority)
{
	switch( priority )
	{
		case IDLE_THREAD:
			return IDLE_PRIORITY_CLASS;
			break;
		case LOWEST_PRIORITY:
			return BELOW_NORMAL_PRIORITY_CLASS;
			break;
		case LOW_PRIORITY:
			return BELOW_NORMAL_PRIORITY_CLASS;
			break;
		case NORMAL_PRIORITY:
			return NORMAL_PRIORITY_CLASS;
			break;
		case HIGH_PRIORITY:
			return ABOVE_NORMAL_PRIORITY_CLASS;
			break;
		case HIGHEST_PRIORITY:
			return HIGH_PRIORITY_CLASS;
			break;
		case CRITICAL_PRIORITY:
			return REALTIME_PRIORITY_CLASS;
			break;
		default:
			return NORMAL_PRIORITY_CLASS;
			break;
	}
}

int Thread::GetCurrentThreadID()
{
	Thread* thread = Self();
	return thread->m_ID;
}

void Thread::SetSelfStorage()
{
	static bool runOnce = true;

	if( runOnce )
	{
		runOnce = false;
		Thread* thread = new Thread;
		s_selfTlsIndex = TlsAlloc();
		TlsSetValue( s_selfTlsIndex, thread );
		thread->m_ID = ::GetCurrentThreadId();
	}
}





