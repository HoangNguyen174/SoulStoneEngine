#ifndef VECTOR4_H
#define VECTOR4_H

class Vector4
{
	public:
		float x;
		float y;
		float z;
		float w;
	
	public:
		Vector4() : x(0.f),y(0.f),z(0.f),w(0.f){};

		Vector4( float x, float y, float z, float w )
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4( const Vector4& other )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}

		~Vector4() {}



};



#endif