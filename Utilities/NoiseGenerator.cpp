#include "NoiseGenerator.hpp"
#include "Matrix44.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

Vector3i NoiseGenerator::gradient3DTable[12] = { Vector3i( 1, 1, 0), Vector3i( -1, 1, 0 ), Vector3i( 1,-1, 0 ), Vector3i( -1,-1, 0 ),
												 Vector3i( 1, 0, 1), Vector3i( -1, 0, 1 ), Vector3i( 1, 0,-1 ), Vector3i( -1, 0,-1 ),
											     Vector3i( 0, 1, 1), Vector3i(  0,-1, 1 ), Vector3i( 0, 1,-1 ), Vector3i(  0,-1, -1 ) };

int NoiseGenerator::p[256] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
						       190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,87,174,
						       20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,
						       220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,18,169,
						       200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,
						       118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,
						       163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,
						       218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,49,192,
						       214,31,181,199,106,157,184,84,204,176,115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,29,
						       24,72,243,141,128,195,78,66,215,61,156,180 };

float NoiseGenerator::LinearBlend( float a, float b, float weight )
{
	return ( 1.f - weight ) * a + weight * b;
}

int NoiseGenerator::FastFloor( float value )
{
	return value > 0 ? static_cast<int>( value ) : static_cast<int>( value )  - 1 ;
}

float NoiseGenerator::SmoothStepHighDegree( float value )
{
	return 6 * value * value * value * value * value - 15 * value * value * value * value + 10 * value * value * value; // 6t^5 - 15t^4 + 10t^3
}

float NoiseGenerator::DotProduct( const Vector3i& a, const Vector3& b )
{
	return ( static_cast<float>( a.x ) * b.x + static_cast<float>( a.y ) * b.y + static_cast<float>( a.z ) * b.z );
}

float NoiseGenerator::ComputePerlinNoiseValueAtPosition3D( const Vector3& position )
{
	int permutationTable[512];

	for( int i = 0; i < 512; i++ )
	{
		permutationTable[i] = p[ i & 255 ];
	}

	int X = FastFloor( position.x );
	int Y = FastFloor( position.y );
	int Z = FastFloor( position.z );

	Vector3 relativePosition = Vector3( position.x - X, position.y - Y, position.z - Z );

	// wrap cells at 255
	X = X & 255; 
	Y = Y & 255; 
	Z = Z & 255;

	int gradient000Index = permutationTable[ X + permutationTable[ Y + permutationTable[Z] ] ] % 12;
	int gradient001Index = permutationTable[ X + permutationTable[ Y + permutationTable[Z + 1] ] ] % 12;
	int gradient010Index = permutationTable[ X + permutationTable[ Y + 1 + permutationTable[Z] ] ] % 12;
	int gradient011Index = permutationTable[ X + permutationTable[ Y + 1 + permutationTable[Z + 1] ] ] % 12;
	int gradient100Index = permutationTable[ X + 1 + permutationTable[ Y + permutationTable[Z] ] ] % 12;
	int gradient101Index = permutationTable[ X + 1 + permutationTable[ Y + permutationTable[Z + 1] ] ] % 12;
	int gradient110Index = permutationTable[ X + 1 + permutationTable[ Y + 1 + permutationTable[Z] ] ] % 12;
	int gradient111Index = permutationTable[ X + 1 + permutationTable[ Y + 1 + permutationTable[Z + 1] ] ] % 12;

	float noise000 = DotProduct( gradient3DTable[ gradient000Index ], relativePosition );
	float noise001 = DotProduct( gradient3DTable[ gradient001Index ], Vector3( relativePosition.x, relativePosition.y, relativePosition.z - 1 ) );
	float noise010 = DotProduct( gradient3DTable[ gradient010Index ], Vector3( relativePosition.x, relativePosition.y - 1, relativePosition.z ) );
	float noise011 = DotProduct( gradient3DTable[ gradient011Index ], Vector3( relativePosition.x, relativePosition.y - 1, relativePosition.z - 1 ) );
	float noise100 = DotProduct( gradient3DTable[ gradient100Index ], Vector3( relativePosition.x - 1, relativePosition.y, relativePosition.z ) );
	float noise101 = DotProduct( gradient3DTable[ gradient101Index ], Vector3( relativePosition.x - 1, relativePosition.y, relativePosition.z - 1 ) );
	float noise110 = DotProduct( gradient3DTable[ gradient110Index ], Vector3( relativePosition.x - 1, relativePosition.y - 1, relativePosition.z ) );
	float noise111 = DotProduct( gradient3DTable[ gradient111Index ], Vector3( relativePosition.x - 1, relativePosition.y - 1, relativePosition.z - 1 ) );

	float u = SmoothStepHighDegree( relativePosition.x );
	float v = SmoothStepHighDegree( relativePosition.y );
	float w = SmoothStepHighDegree( relativePosition.z );

	// interpolate each corners along x
	float noiseX00 = LinearBlend( noise000, noise100, u );
	float noiseX01 = LinearBlend( noise001, noise101, u );
	float noiseX10 = LinearBlend( noise010, noise110, u );
	float noiseX11 = LinearBlend( noise011, noise111, u );

	// interpolate four results along y
	float nXY0 = LinearBlend( noiseX00, noiseX10, v );
	float nXY1 = LinearBlend( noiseX01, noiseX11, v );

	// interpolate two results along z
	float result = LinearBlend( nXY0, nXY1, w );

	return result;
}