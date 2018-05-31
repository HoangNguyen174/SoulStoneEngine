#include "HashBufferJob.hpp"
#include "../Utilities/GameCommon.hpp"
#include "MemoryPoolManager.hpp"

void OnHashBufferJobComplete( Job* finishedJob )
{
	HashBufferJob* job = nullptr;
	job = dynamic_cast<HashBufferJob*>( finishedJob );

	DebuggerPrintf( "Generate hash value %i from buffer successfully.\n", job->m_hashValue );
}

HashBufferJob::HashBufferJob( const std::string& string, unsigned long* hashValue ) : Job()
{
	m_hashValue = 0;
	m_string = string;
	m_buffer = nullptr;
	hashValue = &m_hashValue;
	m_callbackFunction = OnHashBufferJobComplete;
}

HashBufferJob::HashBufferJob( char* buffer, unsigned long* hashValue ) : Job()
{
	m_hashValue = 0;
	m_buffer = buffer;
	hashValue = &m_hashValue;
	m_callbackFunction = OnHashBufferJobComplete;
}

void HashBufferJob::Execute( void* /*args*/ )
{
	unsigned int currentIndex = 0;
	const int bit27 = 134217728; // 2^27

	if( m_buffer != nullptr )
	{
		while( m_buffer != nullptr )
		{
			m_hashValue = ( ( m_hashValue & bit27 ) * 31 ) + currentIndex;
			currentIndex++;
			++m_buffer;
		}
	}
	else if( m_string.size() > 0 )
	{
		for( currentIndex = 0; currentIndex < m_string.size(); currentIndex++ )
		{
			m_hashValue = ( ( m_hashValue & bit27 ) * 31 + m_string.at( currentIndex ) );
		}
	}
}


