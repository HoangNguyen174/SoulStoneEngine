#include "LoadFileJob.hpp"
#include "../Utilities/GameCommon.hpp"
#include "MemoryPoolManager.hpp"

void OnLoadFileComplete( Job* finishedJob )
{
	LoadFileJob* job = dynamic_cast<LoadFileJob*>( finishedJob );

	DebuggerPrintf( "Load file %s into buffer successfully.\n", job->m_fileName.c_str() );
}

LoadFileJob::LoadFileJob( const std::string& fileName, char* buffer ) : Job()
{
	m_fileName = fileName;
	m_buffer = nullptr;
	m_buffer = buffer;
	m_callbackFunction = OnLoadFileComplete;
}

void LoadFileJob::Execute( void* /*args*/ )
{
	FILE* file;
	size_t result;

	fopen_s( &file, m_fileName.c_str(), "rb" );

	if( file == nullptr )
	{
		DebuggerPrintf( "Cannot open file %s. Terminate Thread!\n", m_fileName.c_str() );
		exit(0);
	}

	fseek( file, 0,  SEEK_END );
	m_bufferSize = ftell( file );
	rewind( file );

	m_buffer = new char[m_bufferSize];

	if( m_buffer == nullptr )
	{
		DebuggerPrintf( "Memory Error while reading file %s. Terminate thread!\n", m_fileName.c_str() );
		exit(0);
	}

	result = fread( m_buffer,1, m_bufferSize, file );

	if( (long)result != m_bufferSize )
	{
		DebuggerPrintf( "Reading file %s error. Terminate thread!\n", m_fileName.c_str() );
		exit(0);
	}

	fclose(file);
}



