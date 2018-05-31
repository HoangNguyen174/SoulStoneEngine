#include "AABB2.hpp"

AABB2::AABB2()
{
}

AABB2::AABB2(const Vector2& min, const Vector2& max)
{
	m_mins = min;
	m_maxs = max;
}

AABB2::AABB2(const Vector2i& min, const Vector2i& max)
{
	m_mins = Vector2( float( min.x ),  float( min.y ) );
	m_maxs = Vector2( float( max.x ), float( max.y ) );
}

AABB2::~AABB2()
{
}

void AABB2::GenerateAABB2( const vector<Vector2>& list)
{
	vector<float> x_list;
	vector<float> y_list;
	for(unsigned int i = 0; i < list.size();i++)
	{
		x_list.push_back(list[i].x);
		y_list.push_back(list[i].y);
	}
	m_maxs.x = MathUtilities::GetMax(x_list);
	m_maxs.y = MathUtilities::GetMax(y_list);
	m_mins.x = MathUtilities::GetMin(x_list);
	m_mins.y = MathUtilities::GetMin(y_list);
}

bool AABB2::IsCollideWithAABB( const AABB2& other)
{
	if(    m_maxs.x < other.m_mins.x
		|| m_mins.x > other.m_maxs.x
		|| m_maxs.y < other.m_mins.y
		|| m_mins.y > other.m_maxs.y )
		return false;
	return true;
}

bool AABB2::IsPointInSideAABB2( const Vector2& point )
{
	if ( point.x > m_maxs.x ) return false;
	if ( point.x < m_mins.x ) return false;
	if ( point.y > m_maxs.y ) return false;
	if ( point.y < m_mins.y ) return false;
	return true;
}

bool AABB2::IsCollideWithCircle( const Vector2& center, float radius )
{
	float distSquare = CalDistanceSquareFromPointToAABB2( *this, center );

	return distSquare <= radius * radius;
}

float CalDistanceSquareFromPointToAABB2(const AABB2& aabb, const Vector2& point)
{
	float distSquare = 0.f;

	if( point.x < aabb.m_mins.x )
	{
		distSquare += ( aabb.m_mins.x - point.x ) * ( aabb.m_mins.x - point.x );
	}
	if( point.x > aabb.m_maxs.x )
	{
		distSquare += ( point.x - aabb.m_maxs.x ) * ( point.x - aabb.m_maxs.x );
	}
	if( point.y < aabb.m_mins.y )
	{
		distSquare += ( aabb.m_mins.y - point.y ) * ( aabb.m_mins.y - point.y );
	}
	if( point.y > aabb.m_maxs.y )
	{
		distSquare += ( point.y - aabb.m_maxs.y ) * ( point.y - aabb.m_maxs.y );
	}

	return distSquare;
}


