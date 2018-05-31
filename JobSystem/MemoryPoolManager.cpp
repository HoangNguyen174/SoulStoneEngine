#include <Windows.h>
#include "../Utilities/GameCommon.hpp"
#include "../Render/GLRender.hpp"
#include "MemoryPoolManager.hpp"

#ifdef USE_MEMORY_MANAGER
#undef new 
//operator new overloads
void* operator new( const size_t size, const char* file, int line)
{
	return GetMemoryPoolManager()->Allocate( (int)size, 8, file, line );
}

void* operator new( const size_t size, const size_t align, const char* file, int line) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size, align, file, line  );
}

void* operator new[]( const size_t size, const char* file, int line) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size, 8, file, line  );
}

void* operator new[]( const size_t size, const size_t align, const char* file, int line) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size, align, file, line  );
}

//can't easily overload operator delete
void operator delete( void* ptr ) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

void operator delete[]( void* ptr )
{
	GetMemoryPoolManager()->Free( ptr );
}

//matched to the operator new overload above in case of exceptions thrown during allocation
void operator delete( void* ptr, const char* file, int line) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

void operator delete[]( void* ptr, const char* file, int line) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

void operator delete( void* ptr, const size_t align, const char* file, int line) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

void operator delete[]( void* ptr, const size_t align, const char* file, int line) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

//global new/delete
void* operator new( size_t size ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void* operator new( size_t size, const std::nothrow_t& ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void* operator new( size_t size, size_t align ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void* operator new( size_t size, size_t align, const std::nothrow_t& ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void* operator new[]( size_t size ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void* operator new[]( size_t size, const std::nothrow_t& ) throw()
{
	return GetMemoryPoolManager()->Allocate( (int)size );
}

void operator delete( void* ptr, const std::nothrow_t&) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

void operator delete[]( void* ptr, const std::nothrow_t&) throw()
{
	GetMemoryPoolManager()->Free( ptr );
}

#define new new( __FILE__, __LINE__ )

MemoryPoolManager* GetMemoryPoolManager()
{
	static MemoryPoolManager myMemManager;
	return &myMemManager;
}

int	MemoryPoolManager::s_minFreeBlockSizeInBytes = 16;

void MemoryBlock::WriteTo( void* dest )		
{
	memcpy( dest, this, sizeof( MemoryBlock ) );
}

MemoryPoolManager::MemoryPoolManager()
{
	m_poolMemory = (unsigned char*)malloc( INITIAL_POOL_SIZE_IN_BYTES );
	m_freePoolSizeInBytes = INITIAL_POOL_SIZE_IN_BYTES - sizeof( MemoryBlock );
	m_totalPoolSizeInBytes = INITIAL_POOL_SIZE_IN_BYTES;
	m_largestRequestedMemoryBlock = 0;
	m_averageRequestedMemoryBlock = 0;
	MemoryBlock freeBlock( INITIAL_POOL_SIZE_IN_BYTES - sizeof(MemoryBlock) );
	freeBlock.WriteTo( m_poolMemory );
}

MemoryPoolManager::~MemoryPoolManager()
{
	free( m_poolMemory );
}

void* MemoryPoolManager::Allocate( int sizeInBytes, size_t align, const char* fileName, int fileLine )
{
	if( sizeInBytes < 8 )
		sizeInBytes = 8;

	int actualRequiredSize = sizeInBytes + sizeof( MemoryBlock );

	MemoryBlock* allocatedBlock = ( MemoryBlock* ) m_poolMemory;

	while( allocatedBlock )
	{
		if( allocatedBlock->m_isFree && allocatedBlock->m_userDataSize >= actualRequiredSize )
			break;
		allocatedBlock = allocatedBlock->m_nextBlock;
	}

	BYTE* BlockAddr = (BYTE*)allocatedBlock; 

	if( !allocatedBlock )
	{
		return nullptr;
	}

	int freeMemorySizeAfterAllocation = allocatedBlock->m_userDataSize - actualRequiredSize ;
	if( freeMemorySizeAfterAllocation > s_minFreeBlockSizeInBytes )
	{
		MemoryBlock freeBlock( freeMemorySizeAfterAllocation - sizeof(MemoryBlock) );
		freeBlock.m_nextBlock = allocatedBlock->m_nextBlock;
		freeBlock.m_prevBlock = allocatedBlock;
		freeBlock.WriteTo( BlockAddr + actualRequiredSize );
		if( freeBlock.m_nextBlock )
			freeBlock.m_nextBlock->m_prevBlock = ( MemoryBlock* ) ( BlockAddr + actualRequiredSize );
		allocatedBlock->m_userDataSize = sizeInBytes;
		allocatedBlock->m_nextBlock = (MemoryBlock*)( BlockAddr + actualRequiredSize );
	}

	m_freePoolSizeInBytes -= allocatedBlock->m_userDataSize;
	allocatedBlock->m_isFree = false;
	allocatedBlock->m_fileName = fileName;
	allocatedBlock->m_line = fileLine;

	return ( BlockAddr + sizeof( MemoryBlock) );
}

void MemoryPoolManager::Free( void* ptr )
{
	if( !ptr )
		return;
	MemoryBlock* removeBlock = ( MemoryBlock* )( (BYTE*)ptr - sizeof( MemoryBlock ) );
	if( removeBlock->m_isFree )
	{
		return;
	}

	int fullBlockSize = removeBlock->m_userDataSize + sizeof( MemoryBlock );
	m_freePoolSizeInBytes += removeBlock->m_userDataSize;

	MemoryBlock* freeBlock = removeBlock;
	MemoryBlock* freeBlockPrev = removeBlock->m_prevBlock;
	MemoryBlock* freeBlockNext = removeBlock->m_nextBlock;

	if( removeBlock->m_prevBlock && removeBlock->m_prevBlock->m_isFree )
	{
		freeBlock = removeBlock->m_prevBlock;
		freeBlockPrev = removeBlock->m_prevBlock->m_prevBlock;
		freeBlockNext = removeBlock->m_nextBlock;

		fullBlockSize += ( removeBlock->m_prevBlock->m_userDataSize + sizeof( MemoryBlock ) );

		if( removeBlock->m_nextBlock )
		{
			removeBlock->m_nextBlock->m_prevBlock = freeBlock;

			if( removeBlock->m_nextBlock->m_isFree )
			{
				freeBlockNext = removeBlock->m_nextBlock->m_nextBlock;
				if( removeBlock->m_nextBlock->m_nextBlock )
					removeBlock->m_nextBlock->m_nextBlock->m_prevBlock = freeBlock;

				fullBlockSize += ( removeBlock->m_nextBlock->m_userDataSize + sizeof( MemoryBlock ) );
			}
		}
	}
	else if( removeBlock->m_nextBlock && removeBlock->m_nextBlock->m_isFree )
	{
		freeBlock = removeBlock;
		freeBlockPrev = removeBlock->m_prevBlock;
		freeBlockNext = removeBlock->m_nextBlock->m_nextBlock;
		if( removeBlock->m_nextBlock->m_nextBlock )
			removeBlock->m_nextBlock->m_nextBlock->m_prevBlock = freeBlock;

		fullBlockSize += ( removeBlock->m_nextBlock->m_userDataSize + sizeof( MemoryBlock ) );
	}

	BYTE* freeBlockStartAddr = (BYTE*) freeBlock;
	int freeBlockUserDataSize = fullBlockSize - sizeof( MemoryBlock );
	MemoryBlock freeMemBlock( freeBlockUserDataSize );
	freeMemBlock.m_prevBlock = freeBlockPrev;
	freeMemBlock.m_nextBlock = freeBlockNext;
	freeMemBlock.WriteTo( freeBlockStartAddr );
}

void MemoryPoolManager::CheckMemoryLeaks()
{
	MemoryBlock* memoryBlock = (MemoryBlock*) m_poolMemory;
	static int count = 0;

	while( memoryBlock )
	{
		if( memoryBlock->m_isFree )
		{
			memoryBlock = memoryBlock->m_nextBlock;
			continue;
		}

		if( count == 0 )
		{
			DebuggerPrintf( "-------------------------MEMORY LEAKS CHECKING RESULTS-------------------------\n" );
			count = 1;
		}

		int dataSize = memoryBlock->m_userDataSize;

		if( dataSize == 140 )
		{
			int l = 0;
		}
		const char* size = "Bytes";
		if( dataSize >= 1024 )
		{
			size = "KBs";
			dataSize = dataSize / 1024;
		}
		DebuggerPrintf( "%s (%i): allocated %i %s.\n",  memoryBlock->m_fileName, memoryBlock->m_line, dataSize, size );

		memoryBlock = memoryBlock->m_nextBlock;
	}
}

void MemoryPoolManager::PrintMemoryStatisticToScreen()
{
	int allocatedMemorySize = m_totalPoolSizeInBytes - m_freePoolSizeInBytes;
	
	MemoryBlock* memoryBlock = (MemoryBlock*) m_poolMemory;

	int largestAllocatedSize = 0;
	int averageAllocatedSize = 0;
	int allocatedMemCount = 0;

	while( memoryBlock )
	{
		if( memoryBlock->m_userDataSize > largestAllocatedSize && !memoryBlock->m_isFree )
			largestAllocatedSize = memoryBlock->m_userDataSize;

		if( !memoryBlock->m_isFree )
		{
			allocatedMemCount++;
		}

		memoryBlock = memoryBlock->m_nextBlock;
	}

	if( allocatedMemCount != 0 )
		averageAllocatedSize = allocatedMemorySize / allocatedMemCount;

	int percentFree = 100.f * m_freePoolSizeInBytes / m_totalPoolSizeInBytes;
	int rest = 100.f - percentFree;

	std::string percentFreeStr = std::to_string( (long long) percentFree );
	std::string restPercentStr = std::to_string( (long long) rest );
	std::string MemoryPoolSizeStr = "Memory Pool Size: " + std::to_string( (long long) m_totalPoolSizeInBytes / 1024 ) + " KBs";
	std::string allocatedMemorySizeStr = "Allocated Memory: " + std::to_string( (long long) allocatedMemorySize / 1024 ) + " KBs "  + "(" + restPercentStr + "%)";
	std::string freeMemorySizeStr = "Free Memory: " + std::to_string( (long long) m_freePoolSizeInBytes / 1024) + " KBs " + "(" + percentFreeStr + "%)";
	std::string largestAllocMemorySizeStr = "Largest Allocated Memory: " + std::to_string( (long long) largestAllocatedSize / 1024 ) + " KBs";
	std::string averageMemorySizeStr = "Average Allocated Memory: " + std::to_string( (long long) averageAllocatedSize ) + " Bytes";
	std::string locatedMemCountStr = "Current Number Allocations: " + std::to_string( (long long) allocatedMemCount );


	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.f, WINDOW_VIRTUAL_WIDTH, 0.f, WINDOW_VIRTUAL_HEIGHT, 0.f, 1.f );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.97f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, MemoryPoolSizeStr );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.95f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, allocatedMemorySizeStr );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.93f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, freeMemorySizeStr );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.91f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, largestAllocMemorySizeStr );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.89f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, averageMemorySizeStr );
	GraphicManager::s_render->RenderText( Vector2( 0.8f * WINDOW_VIRTUAL_WIDTH, 0.87f * WINDOW_VIRTUAL_HEIGHT ), RGBColor::White(), nullptr, nullptr, 0.2f, locatedMemCountStr );
	glPopMatrix();
}	

#endif // USE_MEMORY_MANAGER


