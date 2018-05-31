#ifndef XML_UTILITIES_H
#define XML_UTILITIES_H
#include "tinyxml2.h"
#include "RGBColor.hpp"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include "GameCommon.hpp"
#include <string>

class XMLLoaderUtilities
{
	public:
		tinyxml2::XMLDocument m_xmlDataFile;
		std::string m_xmlFileName;
		tinyxml2::XMLElement* m_root;

	private:
		std::string m_xmlFullPath; 

	public:
		XMLLoaderUtilities( const std::string& fileName );
		static float GetFloatXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, float defaultValue );
		static int GetIntXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, int defaultValue );
		static RGBColor GetRGBXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, RGBColor defaultValue );
		static bool GetBoolAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, bool defaultValue );
		static std::string GetStringXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, std::string defaultValue );
		static Vector2 GetVector2XMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector2 defaultValue );
		static Vector2i GetVector2iXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector2i defaultValue );
		static Vector3 GetVector3XMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector3 defaultValue );
		static Vector3i GetVector3iXMLAttribute( const tinyxml2::XMLElement* xmlElement, std::string attributeName, Vector3i defaultValue );
		void ValidateXMLAttributes( const tinyxml2::XMLElement* xmlElement, const::string& requiredAttributes, const::string& optionalAttributes );
		void ValidateXMLChildElements( const tinyxml2::XMLElement* xmlElement, const::string& requiredChildElements, const::string& optionalChildElements );

	private:
		static RGBColor ParseColorString( const std::string& );
		static Vector2 ParseVector2String( const std::string& );
		static Vector3 ParseVector3String( const std::string& );
		static std::vector<std::string> ParseStringWithDelimeter( const std::string& stringToParse, const std::string& delimeter);
		static bool IsAttributeWithNameExist( const tinyxml2::XMLElement* xmlElement,const std::string& attributeName );
		bool IsAttributeNameValid( const std::string& name, const std::vector<std::string>& requiredAttributesList, const std::vector<std::string>& optionalAttributesList );
		bool IsNumberOfRequiredAttributesValid( const std::vector<std::string>& attribNameList, const std::vector<std::string>& requiredAttributesList );
		bool IsChildElementHasValidName( const std::string& name, const std::vector<std::string>& requiredElementList, const std::vector<std::string>& optionalElementList );
		bool IsChildElementHasRequiredElement( const std::string& name );
		void DisplayErrorWindow( char* caption, char* message );
		static std::string GetRealAttributeNameFromLowerCaseName( const tinyxml2::XMLElement* xmlElement, const std::string& attributeName );
};

#endif
