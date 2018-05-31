#include "MutexLock.hpp"
#include "MemoryPoolManager.hpp"

MutexLock::MutexLock( Mutex& mutex )
{
	m_mutex = mutex;
	m_mutex.Lock();
}

MutexLock::~MutexLock()
{
	m_mutex.Unlock();
}