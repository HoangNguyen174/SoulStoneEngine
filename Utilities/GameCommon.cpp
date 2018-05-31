#include "GameCommon.hpp"

DeveloperConsole* g_theConsole = nullptr;
bool IsKeyDownKeyboard[256];
bool IsKeyDownLastFrame[256];

const unsigned int STRINGF_STACK_LOCAL_TEMP_LENGTH = 256;
//-----------------------------------------------------------------------------------------------
//Construct a temporary std::string using printf-style formatting.  For example:
//std::string studentInfoString = Stringf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
//Same as the above version of Stringf(), but with an explicitly chosen maximum buffer size.

const std::string Stringf( const int maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}



//-----------------------------------------------------------------------------------------------
//Print a string to the Visual Studio "Output" debug window, if we are running in the debugger.
//Uses printf-style formatting, for example:
//DebuggerPrintf( "%s is a %d-year-old %s.", studentName, studentAge, studentGender );

void DebuggerPrintf( const char* messageFormat, ... )
{
	const int MESSAGE_MAX_LENGTH = 2048;
	char messageLiteral[ MESSAGE_MAX_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, messageFormat );
	vsnprintf_s( messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, messageFormat, variableArgumentList );
	va_end( variableArgumentList );
	messageLiteral[ MESSAGE_MAX_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	//#if defined( PLATFORM_WINDOWS )
	//	if( IsDebuggerAvailable() )
	//	{
	OutputDebugStringA( messageLiteral ); // Print the string to the "Output" debug window in Visual Studio
	//	}
	//#endif

	std::cout << messageLiteral; // Also write it out to standard out, if anyone is listening
}

TileCoords2D ConvertTileIndexToTileCoords2D( const TileIndex& index,int mapWidthX,int /*mapHeightY*/ )
{
	TileCoords2D tileCoords;

	tileCoords.x = index % mapWidthX;
	tileCoords.y = index / mapWidthX;

	return tileCoords;
}

TileIndex ConvertTileCoords2DToTileIndex( const TileCoords2D& tileCoords,int mapWidthX )
{
	return ( tileCoords.x + mapWidthX * tileCoords.y );
}

WorldCoords2D ConvertTileCoords2DToWorldCoords2D( const TileCoords2D& tileCoords )
{
	return ( Vector2( (float) tileCoords.x, (float)tileCoords.y ) );
}

TileCoords2D ConvertWorldCoords2DToTileCoords2D( const WorldCoords2D& worldCoords )
{
	return ( Vector2i( (int)( floor( worldCoords.x ) ), (int)( floor( worldCoords.y ) ) ) );
}

TileIndex ConvertWorldCoords2DToTileIndex( const WorldCoords2D& worldCoords, int mapWidthX )
{
	TileCoords2D tileCoords = ConvertWorldCoords2DToTileCoords2D( worldCoords );
	
	return ConvertTileCoords2DToTileIndex( tileCoords, mapWidthX );
}

std::string DecapitalizeString( const std::string& str )
{
	std::string temp;
	temp = str;
	for( unsigned int i = 0; i < str.size(); i++ )
	{
		temp[i] = (char)tolower( str[i] );
	}
	return temp;
}

std::string ConvertWStringToUTF8( const std::wstring& wstr )
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}