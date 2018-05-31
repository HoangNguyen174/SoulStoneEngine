#ifndef AABB2_H
#define AABB2_H
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "MathUtilities.hpp"

#undef new
#include <vector>
//#define new new( __FILE__, __LINE__ )

#include <math.h>

class AABB2
{
	public:
		Vector2		m_maxs;
		Vector2		m_mins;

	public:
		AABB2();
		AABB2( const Vector2& min, const Vector2& max );
		AABB2( const Vector2i& min, const Vector2i& max );
		~AABB2();
		void GenerateAABB2( const std::vector<Vector2>& pointsList );
		bool IsCollideWithAABB( const AABB2& other);
		bool IsPointInSideAABB2( const Vector2& point);
		bool IsCollideWithCircle( const Vector2& point, float radius );
		friend float CalDistanceSquareFromPointToAABB2( const AABB2& aabb, const Vector2& point ); 
};
#endif