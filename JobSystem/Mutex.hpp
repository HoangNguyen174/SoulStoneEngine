#ifndef MUTEX_H
#define MUTEX_H
#include <Windows.h>

class Mutex
{
	public:
		CRITICAL_SECTION	m_critSection;

	private:
		int					m_recursionCount;

	public:
		Mutex();
		~Mutex();
		void Lock();
		void Unlock();
		int GetRecursionCount() const;
};

#endif