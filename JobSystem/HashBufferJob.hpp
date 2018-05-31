#ifndef HASH_BUFFER_JOB_H
#define HASH_BUFFER_JOB_H
#include "Job.hpp"
#include <string>

class HashBufferJob : public Job
{
	public:
		char*			m_buffer;
		std::string		m_string;
		unsigned long	m_hashValue;

	public:
		HashBufferJob( char* buffer, unsigned long* hashValue );
		HashBufferJob( const std::string& string, unsigned long* hashValue );
		void Execute( void* args );
};

#endif