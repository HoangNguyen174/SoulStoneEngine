#ifndef GAMECOMMON_HEADER
#define GAMECOMMON_HEADER
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#undef new
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
//#define new new( __FILE__, __LINE__ )

#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
#include <climits>
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include "Vector4.hpp"
#include "RGBColor.hpp"
#include "Texture.hpp"
#include "../Debugger/DeveloperConsole.hpp"
#include "Time.hpp"

typedef Vector2 WorldCoords2D;
typedef Vector2i TileCoords2D;
typedef int TileIndex;
typedef Vector3 WorldCoords3D;
typedef Vector3i TileCoords3D;
typedef Vector2i ChunkCoords2D;

struct Bone
{
	int			m_index;
	float		m_weight;
};

struct Vertex3D
{
	Vector3		m_position;
	RGBColor	m_color;
	Vector2		m_texCoords;
	Vector3		m_normal;
	float		m_terrainTypeWeight[3];
	Vector3		m_tangent;
	Vector3		m_bitangent;
	Vector4		m_boneIndex;
	Vector4		m_boneWeight;
	float		m_isStatic;
};

struct Vertex2D
{
	Vector2		m_position;
	RGBColor	m_color;
	Vector2		m_texCoords;
};

enum RenderType { OPENGL_RENDER, DIRECTX_RENDER };

enum CARDINAL_DIRECTION { NONE = -1,
						  NORTH_CARDINAL_DIRECTION = 0, 
						  SOUTH_CARDINAL_DIRECTION,  
						  EAST_CARDINAL_DIRECTION,
						  WEST_CARDINAL_DIRECTION,
						  NORTH_WEST_SEMICARDINAL_DIRECTION,
						  NORTH_EAST_SEMICARDINAL_DIRECTION,
						  SOUTH_WEST_SEMICARDINAL_DIRECTION,
						  SOUTH_EAST_SEMICARDINAL_DIRECTION = 7 };
//-----------------------------------------------------------------------------------------------
//global variable
extern DeveloperConsole* g_theConsole;
extern const int WINDOW_PHYSICAL_WIDTH;
extern const int WINDOW_PHYSICAL_HEIGHT;
extern const float WINDOW_VIRTUAL_HEIGHT;
extern const float WINDOW_VIRTUAL_WIDTH;
extern HWND g_hWnd;
const float ZOOM_FACTOR = 1.1f;
const float MOUSE_MOVE_RATE = 10.f;
extern bool IsKeyDownKeyboard[256];
extern bool IsKeyDownLastFrame[256];
//-----------------------------------------------------------------------------------------------


const std::string Stringf( const char* format, ... );
const std::string Stringf( const int maxLength, const char* format, ... );
void DebuggerPrintf( const char* messageFormat, ... );
TileCoords2D ConvertTileIndexToTileCoords2D( const TileIndex& index, int mapWidthX, int mapHeightY );
TileIndex ConvertTileCoords2DToTileIndex( const TileCoords2D& tileCoords, int mapWidthX );
WorldCoords2D ConvertTileCoords2DToWorldCoords2D( const TileCoords2D& tileCoords );
TileCoords2D ConvertWorldCoords2DToTileCoords2D( const WorldCoords2D& worldCoords );
TileIndex ConvertWorldCoords2DToTileIndex( const WorldCoords2D& worldCoords, int mapWidthX );
std::string DecapitalizeString( const std::string& str );
std::string ConvertWStringToUTF8( const std::wstring& wstr );
# define ARRAY_SIZE_IN_ELEMENTS(A)	ARRAYSIZE(A)

#endif