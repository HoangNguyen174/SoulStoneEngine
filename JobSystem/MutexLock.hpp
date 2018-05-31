#ifndef MUTEX_LOCK_H
#define MUTEX_LOCK_H
#include "Mutex.hpp"

class MutexLock
{
	private:
		Mutex	m_mutex;

	public:
		MutexLock( Mutex& mutex  );
		~MutexLock();

	private:
		MutexLock() {};
		MutexLock operator=( const Mutex& ) {};
};

#endif