#ifndef EXPORTER_LOADER_H
#define EXPORTER_LOADER_H
#include <string>
#include <vector>
#include "BaseObject.hpp"

class ExporterLoader
{		
	public:
		ExporterLoader() {};
		void ReconstructHierachyStructure( const std::vector<BaseObject*>& objectList, std::vector<BaseObject*>& hierachyObjectList );
		void LoadAndReconstructSceneFromExportFile( const std::string& fileName, std::vector<BaseObject*>& objectList, std::vector<BaseObject*>& boneList );	
};

#endif