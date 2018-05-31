#include "ArchiveManager.hpp"
#include "../Utilities/GameCommon.hpp"
#include "../Utilities/strutil.h"
#include "MemoryPoolManager.hpp"

std::vector<FileInArchiveInfo> ArchiveManager::s_fileInArchiveList;
LoadDataMode				   ArchiveManager::s_loadMode = LOAD_FROM_DISK;

void ArchiveManager::Initialize()
{
	if( s_loadMode == LOAD_FROM_DISK )
		return;

	HZIP handleZipData = OpenZip( _T( "Data.zip" ), "Myengine123!" );

	if( handleZipData == 0 )
	{
		if( s_loadMode == PREFER_LOAD_FROM_ARCHIVE )
			return;

		DebuggerPrintf( "Cannot load Data.zip file. Exit Program.\n" );
		exit(ZR_READ);
	}

	ZIPENTRY zEntry;
	ZRESULT zResult = GetZipItem( handleZipData, -1, &zEntry );
	int numItems = zEntry.index;
	if( zResult != ZR_OK )
	{
		CloseZip( handleZipData );
		DebuggerPrintf( "Error extracting file in Zip.data.\n" );
		exit(zResult);
	}

	for( int itemIndex = 0; itemIndex < numItems; itemIndex++ )
	{
		zResult = GetZipItem( handleZipData, itemIndex, &zEntry );
		if( zResult != ZR_OK )
		{
			CloseZip( handleZipData );
			DebuggerPrintf( "Error extracting file in Data.zip.\n" );
			exit(zResult);
		}

		if ( !( zEntry.attr & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			FileInArchiveInfo fileInfo;
			unsigned char* buffer = nullptr;
			unsigned int bufferSize = zEntry.unc_size + 1;
			std::string name = ConvertWStringToUTF8(zEntry.name);
			std::string extenstion = str::substr( name, -4 );
			fileInfo.fileName = "./" + name;

			buffer = new unsigned char[ bufferSize ];
			buffer[ bufferSize - 1 ] = '\0';
			
			fileInfo.fileSize = bufferSize;
			zResult = UnzipItem( handleZipData, itemIndex, buffer, bufferSize );
			if( zResult != ZR_OK )
			{
				DebuggerPrintf( "Error reading %s data from archive to memory.\n", fileInfo.fileName.c_str() );
				CloseZip( handleZipData );
				exit(0);
			}
			fileInfo.fileContent = buffer;
			s_fileInArchiveList.push_back( fileInfo );
		}
	}	
	CloseZip( handleZipData );
}

FileInArchiveInfo ArchiveManager::LoadFileFromArchive( const std::string& fileName )
{
	FileInArchiveInfo returnFile;

	returnFile.fileSize = -1;

	for( unsigned int fileIndex = 0; fileIndex < s_fileInArchiveList.size(); fileIndex++ )
	{
		returnFile = s_fileInArchiveList[fileIndex];
		if( returnFile.fileName.compare( fileName ) == 0)
			return returnFile;
	}
	return returnFile;
}

bool ArchiveManager::IsArchiveExist()
{
	HZIP handleZipData = OpenZip( _T( "Data.zip" ), "Myengine123!" );

	if( handleZipData == 0 )
		return false;
	return true;
}
