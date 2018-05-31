#include "XMLLoaderUtilities.hpp"
#include "../JobSystem/ArchiveManager.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

XMLLoaderUtilities::XMLLoaderUtilities( const std::string& fileName )
{
	m_xmlFileName = fileName;

	switch( ArchiveManager::s_loadMode )
	{
		case LOAD_FROM_DISK: m_xmlDataFile.LoadFile( m_xmlFileName.c_str() );
							 break;
		case LOAD_FROM_ARCHIVE:
		{
			FileInArchiveInfo xmlFile = ArchiveManager::LoadFileFromArchive( fileName );
			if( xmlFile.fileSize != -1 )
			{
				m_xmlDataFile.Parse( (const char*)(xmlFile.fileContent) );
			}
			break;
		}
		case PREFER_LOAD_FROM_DISK: 
		{
			m_xmlDataFile.LoadFile( m_xmlFileName.c_str() );
			if( m_xmlDataFile.ErrorID() != 0 ) 
			{
				FileInArchiveInfo xmlFile = ArchiveManager::LoadFileFromArchive( fileName );
				if( xmlFile.fileSize != -1 )
				{
					m_xmlDataFile.Parse( (const char*)(xmlFile.fileContent) );
				}
			}
			break;
		}
		case PREFER_LOAD_FROM_ARCHIVE:
		{
			FileInArchiveInfo xmlFile = ArchiveManager::LoadFileFromArchive( fileName );
			if( xmlFile.fileSize != -1 )
			{
				m_xmlDataFile.Parse( (const char*)(xmlFile.fileContent) );
			}
			else
				m_xmlDataFile.LoadFile( m_xmlFileName.c_str() );

			break;
		}
	}

	if( m_xmlDataFile.ErrorID() != 0 ) 
	{
		DebuggerPrintf( "Failed to load %s file.\n", m_xmlFileName.c_str() );
		exit(0);
	}

	m_root = m_xmlDataFile.FirstChildElement();

	if(m_root == nullptr)
	{
		DebuggerPrintf( "Failed to load %s file: No root element.\n", m_xmlFileName.c_str() );
		exit(0);
	}

	char currentDirectory[1024];
	GetCurrentDirectoryA( 1024, currentDirectory );

	m_xmlFileName = m_xmlFileName.substr( 2, m_xmlFileName.size() );
	m_xmlFullPath = currentDirectory + ( "\\" + m_xmlFileName );
}

float XMLLoaderUtilities::GetFloatXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, float defaultValue )
{
	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	return xmlElement->FloatAttribute( name.c_str() );
}

int XMLLoaderUtilities::GetIntXMLAttribute(const tinyxml2::XMLElement* xmlElement, std::string attributeName, int defaultValue)
{
	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	return xmlElement->IntAttribute( name.c_str() );
}

RGBColor XMLLoaderUtilities::GetRGBXMLAttribute(const tinyxml2::XMLElement* xmlElement, std::string attributeName, RGBColor defaultValue)
{
	RGBColor returnColor;
	const char* colorAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName ) )
		return returnColor;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	colorAttributeString = xmlElement->Attribute( name.c_str() );
	returnColor = ParseColorString( colorAttributeString );
	return returnColor;
}

bool XMLLoaderUtilities::GetBoolAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, bool defaultValue )
{
	const char* boolAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
	{
		DebuggerPrintf( "Warning: Bool attribute does not exist, default value is return.\n");
		return defaultValue;
	}

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	boolAttributeString = xmlElement->Attribute( name.c_str() );
	 
	if( strcmp( boolAttributeString, "true" ) == 0 )
		return true;
	else
		return false;
}

std::string XMLLoaderUtilities::GetStringXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, std::string defaultValue )
{
	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	return xmlElement->Attribute( name.c_str() );
}

Vector2 XMLLoaderUtilities::GetVector2XMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector2 defaultValue )
{
	Vector2 returnVector;
	const char* vectorAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName ) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	vectorAttributeString = xmlElement->Attribute( name.c_str() );
	returnVector = ParseVector2String( vectorAttributeString );
	return returnVector;
}

Vector3 XMLLoaderUtilities::GetVector3XMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector3 defaultValue )
{
	Vector3 returnVector;
	const char* vectorAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	vectorAttributeString = xmlElement->Attribute( name.c_str() );
	returnVector = ParseVector3String( vectorAttributeString );
	return returnVector;
}

Vector3i XMLLoaderUtilities::GetVector3iXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector3i defaultValue )
{
	Vector3i returnVector;
	Vector3 temp;
	const char* vectorAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	vectorAttributeString = xmlElement->Attribute( name.c_str() );
	temp = ParseVector3String( vectorAttributeString );

	returnVector.x = (int)temp.x;
	returnVector.y = (int)temp.y;
	returnVector.z = (int)temp.z;

	return returnVector;
}

Vector2i XMLLoaderUtilities::GetVector2iXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector2i defaultValue )
{
	Vector2i returnVector;
	Vector2 temp;
	const char* vectorAttributeString;

	if(xmlElement == nullptr)
	{
		DebuggerPrintf( "Error: XML element is null.\n" );
		exit(0);
	}

	if( !IsAttributeWithNameExist( xmlElement, attributeName) )
		return defaultValue;

	std::string name = GetRealAttributeNameFromLowerCaseName( xmlElement, attributeName );

	vectorAttributeString = xmlElement->Attribute( name.c_str() );
	temp = ParseVector2String( vectorAttributeString );

	returnVector.x = (int)temp.x;
	returnVector.y = (int)temp.y;

	return returnVector;
}

std::string XMLLoaderUtilities::GetRealAttributeNameFromLowerCaseName( const tinyxml2::XMLElement* xmlElement, const std::string& attributeName )
{
	for( const tinyxml2::XMLAttribute* attribute = xmlElement->FirstAttribute(); attribute != nullptr; attribute = attribute->Next() )
	{
		std::string temp = attribute->Name();
		if( DecapitalizeString( temp ).compare( DecapitalizeString( attributeName ) ) == 0 )
			return temp;
	}
	exit(0);
}

Vector3 XMLLoaderUtilities::ParseVector3String( const std::string& vectorString )
{
	Vector3 returnVector;
	std::vector<string> vector3StringToExtract;

	vector3StringToExtract = ParseStringWithDelimeter( vectorString, "," );

	if( vector3StringToExtract.size() != 3 )
	{
		DebuggerPrintf( "Error: Too Few/Too Much Components from Vector String.\n");
		exit(0);
	}

	returnVector.x = (float)atof( vector3StringToExtract[0].c_str() );
	returnVector.y = (float)atof( vector3StringToExtract[1].c_str() ); 
	returnVector.z = (float)atof( vector3StringToExtract[2].c_str() ); 

	return returnVector;
}

Vector2 XMLLoaderUtilities::ParseVector2String( const std::string& vectorString )
{
	Vector2 returnVector;
	std::vector<string> vector2StringToExtract;

	vector2StringToExtract = ParseStringWithDelimeter( vectorString, "," );

	if( vector2StringToExtract.size() != 2 )
	{
		DebuggerPrintf( "Error: Few/Too Much Components from Vector String.\n");
		exit(0);
	}

	returnVector.x = (float)atof( vector2StringToExtract[0].c_str() );
	returnVector.y = (float)atof( vector2StringToExtract[1].c_str() ); 

	return returnVector;
}

RGBColor XMLLoaderUtilities::ParseColorString( const std::string& colorString )
{
	RGBColor returnColor;
	std::vector<string> colorStringToExtract;

	colorStringToExtract = ParseStringWithDelimeter( colorString, "," );

	if( colorStringToExtract.size() != 4 )
	{
		DebuggerPrintf( "Error: Too Few/Too Much Components from Color String.\n");
		exit(0);
	}

	returnColor.m_red = (float)atof( colorStringToExtract[0].c_str() );
	returnColor.m_green = (float)atof( colorStringToExtract[1].c_str() );
	returnColor.m_blue = (float)atof( colorStringToExtract[2].c_str() );
	returnColor.m_alpha = (float)atof( colorStringToExtract[3].c_str() );

	return returnColor;
}

bool XMLLoaderUtilities::IsAttributeWithNameExist( const tinyxml2::XMLElement* xmlElement,const std::string& attributeName )
{
	std::vector<std::string> attributeList;
	
	for( const tinyxml2::XMLAttribute* attribute = xmlElement->FirstAttribute(); attribute != nullptr; attribute = attribute->Next() )
	{
		std::string temp = attribute->Name();
		temp = DecapitalizeString( temp );
		attributeList.push_back( temp );
	}

	for( unsigned int i = 0; i < attributeList.size(); i++ )
	{
		if( DecapitalizeString( attributeName ).compare( attributeList[i] ) == 0 )
		{
			return true;
		}
	}
	return false;

// 	const char* attributeNameCStr = attributeName.c_str();
// 	const tinyxml2::XMLAttribute* attributeNameCheck = xmlElement->FindAttribute( attributeNameCStr );
// 
// 	if( attributeNameCheck == nullptr )
// 		return false;
// 
// 	return true;
}

std::vector<std::string> XMLLoaderUtilities::ParseStringWithDelimeter(const std::string& stringToParse, const std::string& delimeter)
{
	unsigned int delimeterPosition = 0;
	std::string tempString = stringToParse;
	std::string stringToken;
	std::vector<std::string> returnStringTokenVector;

	while( ( delimeterPosition = tempString.find( delimeter )) != std::string::npos ) 
	{
		stringToken = tempString.substr( 0, delimeterPosition );
		returnStringTokenVector.push_back( stringToken );
		tempString.erase( 0, delimeterPosition + delimeter.length() );
	}
	if( tempString.compare("") != 0)
		returnStringTokenVector.push_back( tempString );

	return returnStringTokenVector;
}

bool XMLLoaderUtilities::IsAttributeNameValid( const std::string& name, const std::vector<std::string>& requiredAttributesList, const std::vector<std::string>& optionalAttributesList )
{
	char caption[256];
	char errorMessage[2048];
	std::vector<std::string> attributeNameList;
	bool invalidName = false;
	
	for( unsigned int i = 0; i < requiredAttributesList.size(); i++ )
	{
		attributeNameList.push_back( DecapitalizeString( requiredAttributesList[i] ) );
	}

	for( unsigned int i = 0; i < optionalAttributesList.size(); i++ )
	{
		attributeNameList.push_back( DecapitalizeString( optionalAttributesList[i] ) );
	}

	std::string temp = DecapitalizeString( name );
	for( unsigned int i = 0; i < attributeNameList.size();  i++ )
	{
		if( temp.compare( attributeNameList[i] ) == 0 )
		{
			invalidName = false;
			break;
		}
		invalidName = true;
	}

	//construct error message here
	if( invalidName )
	{
		strcpy_s( caption, m_xmlFileName.c_str() );
		strcat_s( caption, " Invalid Attribute/Element Name!" );

		strcpy_s( errorMessage, "File Location:\n");
		strcat_s( errorMessage, m_xmlFullPath.c_str() );
		strcat_s( errorMessage, "\n\n" );
		strcat_s( errorMessage, "Error: Your Attribute/Element name does not match any Required Attributes or Optional Attributes.\n" );
		strcat_s( errorMessage, "\n" );
		strcat_s( errorMessage, "User Input: <" );
		strcat_s( errorMessage, name.c_str() );
		strcat_s( errorMessage, "> \n\n" );
		strcat_s( errorMessage, "Required Attributes/Elements: ");
		for( unsigned int i = 0; i < requiredAttributesList.size(); i++ )
		{
			strcat_s( errorMessage, "<" );
			strcat_s( errorMessage, requiredAttributesList[i].c_str() );
			strcat_s( errorMessage, ">" );
			strcat_s( errorMessage, " " );
		}
		strcat_s( errorMessage, "\n\n" );
		strcat_s( errorMessage, "Optional Attributes/Elements: ");
		if( optionalAttributesList.size() == 0 )
		{
			strcat_s( errorMessage, "No optional attribute allowed." );
		}
		for( unsigned int i = 0; i < optionalAttributesList.size(); i++ )
		{
			strcat_s( errorMessage, "<" );
			strcat_s( errorMessage, optionalAttributesList[i].c_str() );
			strcat_s( errorMessage, ">" );
			strcat_s( errorMessage, " " );
		}
		strcat_s( errorMessage, "\n\n" );
		
		DisplayErrorWindow( caption, errorMessage );
		return false;
	}

	return true;
}

bool XMLLoaderUtilities::IsNumberOfRequiredAttributesValid( const std::vector<std::string>& userInputAttribList, const std::vector<std::string>& requiredAttributesList )
{
//	std::vector<string> temp = attribNameList;
	char caption[256];
	char errorMessage[2048];
	int validNumOfRequiredAttrib = requiredAttributesList.size(); 
	int validRequiredAttribFromUserCounter = 0;

	for( unsigned int i = 0; i < userInputAttribList.size(); i++ )
	{
		std::string userInput = userInputAttribList[i];
		for( unsigned int j = 0; j < requiredAttributesList.size(); j++ )
		{
			std::string requiredAttrib = requiredAttributesList[j];
			if( DecapitalizeString( userInput ).compare( DecapitalizeString( requiredAttrib )) == 0)
			{
				validRequiredAttribFromUserCounter++;
			}
		}
	}

	if( validRequiredAttribFromUserCounter != validNumOfRequiredAttrib )
	{
		//construct error message here
		strcpy_s( caption, m_xmlFileName.c_str() );
		strcat_s( caption, " Invalid Required Attributes!" );

		strcpy_s( errorMessage, "File Location:\n");
		strcat_s( errorMessage, m_xmlFullPath.c_str() );
		strcat_s( errorMessage, "\n\n" );
		strcat_s( errorMessage, "Error: Invalid Number of Required Attributes.\n" );
		strcat_s( errorMessage, "\n" );
		strcat_s( errorMessage, "User input: " );
		for( unsigned int i = 0; i < userInputAttribList.size(); i++ )
		{
			strcat_s( errorMessage, "<" );
			strcat_s( errorMessage, userInputAttribList[i].c_str() );
			strcat_s( errorMessage, "> " );
		}
		strcat_s( errorMessage, "\n\n" );
		strcat_s( errorMessage, "Required Attributes: " );

		for( unsigned int i = 0; i < requiredAttributesList.size(); i++ )
		{
			strcat_s( errorMessage, "<" );
			strcat_s( errorMessage, requiredAttributesList[i].c_str() );
			strcat_s( errorMessage, "> " );
		}
		strcat_s( errorMessage, "\n" );

		DisplayErrorWindow( caption, errorMessage );
		return false;
	}

	return true;
}

void XMLLoaderUtilities::ValidateXMLAttributes( const tinyxml2::XMLElement* xmlElement, const::string& requiredAttributes, const::string& optionalAttributes  )
{
	std::vector<std::string> requiredAttributesList;
	std::vector<std::string> optionAttibutesList;
	std::vector<std::string> attributeNameList;

	if( requiredAttributes.compare("") != 0)
		requiredAttributesList = ParseStringWithDelimeter( requiredAttributes, "," );
	if( optionalAttributes.compare("") != 0)
		optionAttibutesList = ParseStringWithDelimeter( optionalAttributes, "," );
	
	for( const tinyxml2::XMLAttribute* attribute = xmlElement->FirstAttribute(); attribute != nullptr; attribute = attribute->Next() )
	{
		std::string attribName = attribute->Name();

		if( !IsAttributeNameValid( attribName, requiredAttributesList, optionAttibutesList) )
		{
			exit(0);
		}

		attributeNameList.push_back( attribName );
	}

	if( !IsNumberOfRequiredAttributesValid( attributeNameList, requiredAttributesList) )
	{
		exit(0);
	}
}

void XMLLoaderUtilities::ValidateXMLChildElements( const tinyxml2::XMLElement* xmlElement, const::string& requiredChildElement, const::string& optionalChildElement )
{
	std::vector<std::string> requiredAttributesList;
	std::vector<std::string> optionAttibutesList;

	requiredAttributesList = ParseStringWithDelimeter( requiredChildElement, "," );
	optionAttibutesList = ParseStringWithDelimeter( optionalChildElement, "," );

	for( const tinyxml2::XMLElement* firstChildElement = xmlElement->FirstChildElement(); firstChildElement != nullptr; firstChildElement = firstChildElement->NextSiblingElement() )
	{
		std::string childElementName = firstChildElement->Value();
		if( !IsAttributeNameValid( childElementName, requiredAttributesList, optionAttibutesList) )
		{
			exit(0);
		}

		if( !IsNumberOfRequiredAttributesValid( requiredAttributesList, requiredAttributesList) )
		{
			exit(0);
		}
	}
}

void XMLLoaderUtilities::DisplayErrorWindow( char* caption, char* message )
{
	int msgBoxID = MessageBoxA( NULL, 
								message,
								caption,
								MB_ICONSTOP | MB_OK | MB_DEFBUTTON1 );
	if( msgBoxID == IDOK )
	{
		exit(0);
	}
}
