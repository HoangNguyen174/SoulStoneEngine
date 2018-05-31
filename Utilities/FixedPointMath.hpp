#ifndef FIXED_POINT_MATH_H
#define FIXED_POINT_MATH_H
#include <iostream>
#include "MathUtilities.hpp"

template<typename IntegerType, int precision>
class FixedPoint
{
	public:
		IntegerType				m_value;
	
	public:
		explicit FixedPoint() {};
		explicit FixedPoint( int number ) : m_value( number << precision ) {}
		explicit FixedPoint( float number ) : m_value( number * ( 1 << precision ) ) {}
		explicit FixedPoint( double number ) : m_value( (float)number * ( 1 << precision ) ) {}
		~FixedPoint() {};

		FixedPoint operator+()
		{
			FixedPoint returnValue;
			returnValue.m_value = m_value;
			return returnValue;
		}

		FixedPoint operator-()
		{
			FixedPoint returnValue;
			returnValue.m_value = -m_value;
			return returnValue;
		}

		bool operator!()
		{
			return *this == 0;
		}

		FixedPoint& operator+=( const FixedPoint& other )
		{
			m_value += other.m_value;
			return *this;
		}
	
		FixedPoint& operator-=( const FixedPoint& other )
		{
			m_value -= other.m_value;
			return *this;
		}
	
		FixedPoint& operator*=( const FixedPoint& other )
		{
			m_value = (int32_t)(((int64_t)m_value * other.m_value) >> precision );
			return *this;
		}
	
		FixedPoint& operator/=( const FixedPoint& other )
		{
			m_value = (int32_t)((((int64_t)m_value) << precision ) / other.m_value );
			return *this;
		}

		FixedPoint& operator%=( const FixedPoint& other )
		{
			m_value = m_value % other.m_value;
			return *this;
		}

		FixedPoint operator +( const FixedPoint& other ) 
		{
			FixedPoint fixedPoint = *this;
			fixedPoint += other;
			return fixedPoint;
		}

		FixedPoint operator -( const FixedPoint& other ) 
		{
			FixedPoint fixedPoint = *this;
			fixedPoint -= other;
			return fixedPoint;
		}

		FixedPoint operator *( const FixedPoint& other ) 
		{
			FixedPoint fixedPoint = *this;
			fixedPoint *= other;
			return fixedPoint;
		}

		FixedPoint operator /( const FixedPoint& other ) 
		{
			FixedPoint fixedPoint = *this;
			fixedPoint /= other;
			return fixedPoint;
		}

		FixedPoint operator %( const FixedPoint& other )
		{
			FixedPoint fixedPoint = *this;
			fixedPoint %= other;
			return fixedPoint;
		}

		bool operator ==( const FixedPoint& other )
		{
			return m_value == other.m_value;
		}

		bool operator !=( const FixedPoint& other )
		{
			return !(*this != other);
		}

		bool operator <( const FixedPoint& other )
		{
			return m_value < other.m_value;
		}

		bool operator >( const FixedPoint& other )
		{
			return m_value > other.m_value;
		}

		bool operator <=( const FixedPoint& other )
		{
			return m_value <= other.m_value;
		}

		bool operator >=( const FixedPoint& other )
		{
			return m_value >= other.m_value;
		}

		bool operator &&( const FixedPoint& other )
		{
			return m_value && other.m_value;
		}

		bool operator ||( const FixedPoint& other )
		{
			return m_value || other.m_value;
		}

		FixedPoint& operator<<=( int num )
		{
			m_value <<= num;
			return *this;
		}

		FixedPoint& operator<<( int num )
		{
			FixedPoint returnValue = *this;
			return ( returnValue <<= num );
		}

		FixedPoint& operator>>=( int num )
		{
			m_value >>= num;
			return *this;
		}

		FixedPoint& operator>>( int num )
		{
			FixedPoint returnValue = *this;
			return ( returnValue >>= num );
		}

		FixedPoint& operator&=( const FixedPoint& other )
		{
			m_value &= other.m_value;
			return *this;
		}

		FixedPoint& operator&( const FixedPoint& other )
		{
			FixedPoint returnValue = *this;
			returnValue &= other;
			return returnValue;
		}

		FixedPoint& operator|=( const FixedPoint& other )
		{
			m_value |= other.m_value;
			return *this;
		}

		FixedPoint& operator|( const FixedPoint& other )
		{
			FixedPoint returnValue = *this;
			returnValue |= other;
			return returnValue;
		}

		FixedPoint& operator^=( const FixedPoint& other )
		{
			m_value ^= other.m_value;
			return *this;
		}

		FixedPoint& operator^( const FixedPoint& other )
		{
			FixedPoint returnValue = *this;
			returnValue ^= other;
			return returnValue;
		}
 
		FixedPoint& operator~()
		{
			m_value = ~m_value;
			return *this;
		}

		friend ostream& operator<<( ostream& outStream, const FixedPoint& fix )
		{
			outStream << fix.m_value;
		}

};

//----float-----
template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( const FixedPoint<IntegerType,precision>& fix, float num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += floatToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( float num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += floatToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( float num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = floatToFix<IntegerType,precision>(num);
	temp -= fix;
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( const FixedPoint<IntegerType,precision>& fix, float num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp -= floatToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( float num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = floatToFix<IntegerType,precision>(num);
	temp *= fix;
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( const FixedPoint<IntegerType,precision>& fix, float num )
{
	FixedPoint<IntegerType,precision> temp = floatToFix<IntegerType,precision>(num);
	temp *= fix;
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( const FixedPoint<IntegerType,precision>& fix, float num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp /= floatToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( float num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = floatToFix<IntegerType,precision>(num);
	temp /= fix;
	return temp;
}

//----unsigned integer-----
template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( unsigned int num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( const FixedPoint<IntegerType,precision>& fix, unsigned int num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += intToFix<IntegerType,precision>(num);
	return temp; 
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( unsigned int num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = intToFix<IntegerType,precision>(num);
	temp -= fix; 
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( const FixedPoint<IntegerType,precision>& fix, unsigned int num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp -= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( unsigned int num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp *= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( const FixedPoint<IntegerType,precision>& fix, unsigned int num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp *= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( unsigned int num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = intToFix<IntegerType,precision>(num);
	temp /= fix;
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( const FixedPoint<IntegerType,precision>& fix, unsigned int num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp /= intToFix<IntegerType,precision>(num);
	return temp;
}

//----integer-----
template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( int32_t	num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator+( const FixedPoint<IntegerType,precision>& fix, int32_t num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp += intToFix<IntegerType,precision>(num);
	return temp; 
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( int32_t	num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = intToFix<IntegerType,precision>(num);
	temp -= fix; 
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator-( const FixedPoint<IntegerType,precision>& fix, int32_t num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp -= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( int32_t	num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp *= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator*( const FixedPoint<IntegerType,precision>& fix, int32_t num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp *= intToFix<IntegerType,precision>(num);
	return temp;
}

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( int32_t	num, const FixedPoint<IntegerType,precision>& fix )
{
	FixedPoint<IntegerType,precision> temp = intToFix<IntegerType,precision>(num);
	temp /= fix;
	return temp;
}
 
template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> operator/( const FixedPoint<IntegerType,precision>& fix, int32_t num )
{
	FixedPoint<IntegerType,precision> temp = fix;
	temp /= intToFix<IntegerType,precision>(num);
	return temp;
}

//------helper function------------------
template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> intToFix( int32_t num )
{
	FixedPoint<IntegerType,precision> returnValue;
	returnValue.m_value = num << precision;
	return  returnValue;
};

template<typename IntegerType,int precision>
inline int fixToInt( const FixedPoint<IntegerType,precision>& fixNum )
{
	int temp = fixNum.m_value;
	return temp >> precision;
};

template<typename IntegerType,int precision>
inline FixedPoint<IntegerType,precision> floatToFix( float num )
{
	FixedPoint<IntegerType,precision> returnValue;
	returnValue.m_value = num * ( 1 << precision );
	return  returnValue;
};

template<typename IntegerType,int precision>
inline float fixToFloat( const FixedPoint<IntegerType,precision>& fixNum )
{
	return (float)fixNum.m_value / ( 1 << precision );
};

// template<typename IntegerType,int precision>
// inline int floatToFix( float num )
// {
// 	return (int)(num * ( 1 << precision ) );
// }

template<typename IntegerType,int precision>
FixedPoint<IntegerType,precision> power( FixedPoint<IntegerType,precision>& num, int power )
{
	FixedPoint<IntegerType,precision> returnValue(1);

	for( int index = 0; index < power; index++ )
	{
		returnValue *= num;
	}

	return returnValue;
}

template<typename IntegerType,int precision>
FixedPoint<IntegerType,precision> factorial( FixedPoint<IntegerType,precision>& num )
{
	FixedPoint<IntegerType,precision> returnValue(0);

	int temp = fixToInt<IntegerType,precision>( num );

	temp = MathUtilities::Factorial( temp );

	returnValue = intToFix<IntegerType,precision>( temp );

	return returnValue;
}

template<typename IntegerType,int precision>
FixedPoint<IntegerType,precision> sin( FixedPoint<IntegerType,precision>& fixRadian )
{
	FixedPoint<IntegerType,precision> returnValue(0);
	int iteration = 0;
	const int MAX_ITERATION = 10;

	int count = 1;

	for( iteration = 1;  iteration < MAX_ITERATION; iteration += 2 )
	{
		FixedPoint<IntegerType,precision> temp(0);
		long long factorialPart = 0;
		temp = power( fixRadian, iteration );
		factorialPart = MathUtilities::Factorial( iteration );
		if( count % 2 == 0 )
			returnValue -= ( temp / intToFix<IntegerType,precision>( factorialPart ) );
		else
			returnValue += ( temp / intToFix<IntegerType,precision>( factorialPart ) );
		count++;
	}


	return returnValue;
}

template<typename IntegerType,int precision>
FixedPoint<IntegerType,precision> cos( FixedPoint<IntegerType,precision>& fixRadian )
{
	FixedPoint<IntegerType,precision> returnValue(0);
	int iteration = 0;
	const int MAX_ITERATION = 10;

	int count = 1;

	for( iteration = 0;  iteration < MAX_ITERATION; iteration += 2 )
	{
		FixedPoint<IntegerType,precision> temp(0);
		long long factorialPart = 0;
		temp = power( fixRadian, iteration );
		factorialPart = MathUtilities::Factorial( iteration );
		if( count % 2 == 0 )
			returnValue -= temp / intToFix<IntegerType,precision>( factorialPart );
		else
			returnValue += temp / intToFix<IntegerType,precision>( factorialPart );
		count++;
	}


	return returnValue;
}

#endif