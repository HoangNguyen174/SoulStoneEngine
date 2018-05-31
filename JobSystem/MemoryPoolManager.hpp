#ifndef MEMORY_POOL_MANAGER
#define MEMORY_POOL_MANAGER

//#define USE_MEMORY_MANAGER
#ifdef USE_MEMORY_MANAGER
#include <new>

class MemoryBlock
{
	public:
		MemoryBlock*	m_nextBlock;
		MemoryBlock*	m_prevBlock;
		int				m_userDataSize;
		bool			m_isFree;
		const char*		m_fileName;
		int				m_line;

	public:
		MemoryBlock( int dataSize ) : m_nextBlock(nullptr),
									  m_prevBlock(nullptr),
									  m_userDataSize(dataSize),
									  m_isFree(true),
									  m_fileName(nullptr),
									  m_line(0) {};
		void WriteTo( void* dest );
};

const size_t INITIAL_POOL_SIZE_IN_BYTES = 1024 * 1024 * 500;

class MemoryPoolManager
{
	public:
		int				m_totalPoolSizeInBytes;
		int				m_freePoolSizeInBytes;
		int				m_largestRequestedMemoryBlock;
		int				m_averageRequestedMemoryBlock;
		unsigned char*	m_poolMemory;
		static int		s_minFreeBlockSizeInBytes;


	public:
		MemoryPoolManager();
		~MemoryPoolManager();
		void* Allocate( int sizeInBytes, const size_t align = 8, const char* fileName = nullptr, int fileLine = 0);
		void  Free( void* ptr );
		int	  GetFreePoolSizeInBytes();
		void  CheckMemoryLeaks();
		void  PrintMemoryStatisticToScreen();
};

MemoryPoolManager* GetMemoryPoolManager();

void* operator new( const size_t size, const char* file, int line);
void* operator new( const size_t size, const size_t align, const char* file, int line) throw();
void* operator new[]( const size_t size, const char* file, int line) throw();
void* operator new[]( const size_t size, const size_t align, const char* file, int line) throw(); 
void operator delete( void* ptr ) throw();
void operator delete[]( void* ptr ) throw();
void operator delete( void* ptr, const char* file, int line) throw();
void operator delete[]( void* ptr, const char* file, int line) throw();
void operator delete( void* ptr, const size_t align, const char* file, int line) throw();
void operator delete[]( void* ptr, const size_t align, const char* file, int line) throw();
void* operator new( size_t size ) throw();
void* operator new( size_t size, const std::nothrow_t& ) throw();
void* operator new( size_t size, size_t align ) throw();
void* operator new( size_t size, size_t align, const std::nothrow_t& ) throw();
void* operator new[]( size_t size ) throw();
void* operator new[]( size_t size, const std::nothrow_t& ) throw();
void operator delete( void* ptr, const std::nothrow_t&) throw();
void operator delete[]( void* ptr, const std::nothrow_t&) throw();

#define new new( __FILE__, __LINE__ )

#endif // USE_MEMORY_MANAGER

#endif // MEMORY_POOL_MANAGER
