#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H
#include "Vector3.h"
#include "Vector3i.hpp"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Noise.hpp"

class NoiseGenerator
{
	public:
		static Vector3i gradient3DTable[12];
		static int p[256];
	
	private:
		static float	SmoothStepHighDegree( float value );
		static float	LinearBlend( float a, float b, float t );
		static int	    FastFloor( float value );
		static float	DotProduct( const Vector3i& a, const Vector3& b );

	public:
		static float	ComputePerlinNoiseValueAtPosition3D( const Vector3& position );
};

#endif