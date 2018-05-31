#ifndef AABB3_H
#define AABB3_H

#include "Vector3.h"

class AABB3
{
	public:
		Vector3		m_min;
		Vector3		m_max;

	public:
		AABB3();
		AABB3( const Vector3& min, const Vector3& max );
};

#endif