#include "AABB3.hpp"

AABB3::AABB3()
{
}

AABB3::AABB3(const Vector3& min, const Vector3& max)
{
	m_min = min;
	m_max = max;
}