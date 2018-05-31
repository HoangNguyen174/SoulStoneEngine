#include "Mutex.hpp"
#include "MemoryPoolManager.hpp"

Mutex::Mutex()
{
	InitializeCriticalSection( &m_critSection );
}

Mutex::~Mutex()
{
	DeleteCriticalSection( &m_critSection );
}

void Mutex::Lock()
{
	EnterCriticalSection( &m_critSection );
	m_recursionCount++;
}

void Mutex::Unlock()
{
	m_recursionCount--;
	LeaveCriticalSection( &m_critSection );
}

int Mutex::GetRecursionCount() const
{
	return m_recursionCount;
}



