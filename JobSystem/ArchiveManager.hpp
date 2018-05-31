#ifndef ARCHIVE_MANAGER_H
#define ARCHIVE_MANAGER_H
#include <windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include "../ThirdParty/zip/unzip.h"
#include "../ThirdParty/zip/zip.h"

#undef new
#include <vector>
//#define new new( __FILE__, __LINE__ )

struct FileInArchiveInfo
{
	std::string			fileName;
	long				fileSize;
	unsigned char*		fileContent;
};

enum LoadDataMode { LOAD_FROM_ARCHIVE, LOAD_FROM_DISK, PREFER_LOAD_FROM_DISK, PREFER_LOAD_FROM_ARCHIVE };

class ArchiveManager
{
	public:
		static std::vector<FileInArchiveInfo>	s_fileInArchiveList;
		static LoadDataMode						s_loadMode;

	public:
		static void Initialize();
		static bool IsArchiveExist();
		static FileInArchiveInfo LoadFileFromArchive( const std::string& fileName );
};


#endif