#ifndef LOAD_FILE_JOB_H
#define LOAD_FILE_JOB_H
#include "Job.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>

class LoadFileJob : public Job
{
	public:
		char*			m_buffer;
		std::string		m_fileName;
		long			m_bufferSize;

	public:
		LoadFileJob( const std::string& fileName, char* buffer );
		void Execute( void* args );
};

void OnLoadFileComplete( Job* finishedJob );

#endif