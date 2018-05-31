#include "Matrix44.hpp"
#include "../JobSystem/MemoryPoolManager.hpp"

Matrix44::Matrix44()
{
	float temp[16] = { 1.f, 0.f, 0.f, 0.f,  
					   0.f, 1.f, 0.f, 0.f,
		               0.f, 0.f, 1.f, 0.f,
		               0.f, 0.f, 0.f, 1.f }; 

	for(unsigned int i = 0; i < 16; i++)
		m_matrix[i] = temp[i];
}

Matrix44::Matrix44(const float matrix[16])
{
	for(unsigned int i = 0; i < 16; i++)
		m_matrix[i] = matrix[i];
}

Matrix44 Matrix44::CreateIdentity()
{
	Matrix44 temp;
	float iden[16] = { 1.f, 0.f, 0.f, 0.f,
					   0.f, 1.f, 0.f, 0.f,
					   0.f, 0.f, 1.f, 0.f,
					   0.f, 0.f, 0.f, 1.f };

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = iden[i];
	}

	return temp;
}

Matrix44 Matrix44::CreateTranslationMatrix(const Vector3& translation)
{
	Matrix44 translationMatrix;
	translationMatrix.m_matrix[12] = translation.x;
	translationMatrix.m_matrix[13] = translation.y;
	translationMatrix.m_matrix[14] = translation.z;
	return translationMatrix;
}

Matrix44 Matrix44::CreateUniformScaleMatrix(float scale)
{
	Matrix44 scaleMatrix;
	scaleMatrix.m_matrix[0] = scale;
	scaleMatrix.m_matrix[5] = scale;
	scaleMatrix.m_matrix[10] = scale;
	return scaleMatrix;
}

Matrix44 Matrix44::CreateNonUniformScaleMatrix( const Vector3& scaleVector )
{
	Matrix44 nonUniforScaleMatrix;
	nonUniforScaleMatrix.m_matrix[0] = scaleVector.x;
	nonUniforScaleMatrix.m_matrix[5] = scaleVector.y;
	nonUniforScaleMatrix.m_matrix[10] = scaleVector.z;
	return nonUniforScaleMatrix;
}

Matrix44 Matrix44::CreateRotationAboutAxisDegrees(const Vector3& rotationAxis, float rotationDegree )
{
	Vector3 normalizedAxis = rotationAxis;
	normalizedAxis = normalizedAxis.Normalize();
	Vector3& u = normalizedAxis;

	Matrix44 rotationMatrix;

	float cosAngle = cos(MathUtilities::DegToRad(rotationDegree));
	float sinAngle = sin(MathUtilities::DegToRad(rotationDegree));
	float ic = 1.f - cosAngle;

	rotationMatrix.m_matrix[0] = cosAngle + u.x * u.x * ic;
	rotationMatrix.m_matrix[4] = u.x * u.y * ic - u.z * sinAngle;
	rotationMatrix.m_matrix[8] = u.x * u.z * ic + u.y * sinAngle;

	rotationMatrix.m_matrix[1] = u.y * u.x * ic + u.z * sinAngle;
	rotationMatrix.m_matrix[5] = cosAngle + u.y * u.y * ic;
	rotationMatrix.m_matrix[9] = u.y * u.z * ic - u.x * sinAngle;

	rotationMatrix.m_matrix[2] = u.z * u.x * ic - u.y * sinAngle;
	rotationMatrix.m_matrix[6] = u.z * u.y * ic + u.x * sinAngle;
	rotationMatrix.m_matrix[10] = cosAngle + u.z * u.z * ic;

	return rotationMatrix;
}

Matrix44 Matrix44::operator+(const Matrix44& other)
{
	Matrix44 temp;

	for(unsigned int i = 0; i < 16; i++)
		temp.m_matrix[i] = m_matrix[i] + other.m_matrix[i];

	return temp;
}

Matrix44 Matrix44::operator+=(const Matrix44& other)
{
	Matrix44 temp;
	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = m_matrix[i];
	}

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] += other.m_matrix[i];
	}

	return temp;
}

Matrix44 Matrix44::operator-(const Matrix44& other)
{
	Matrix44 temp;

	for(unsigned int i = 0; i < 16; i++)
		temp.m_matrix[i] = m_matrix[i] - other.m_matrix[i];

	return temp;
}

Matrix44 Matrix44::operator-=(const Matrix44& other)
{
	Matrix44 temp;
	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] = m_matrix[i];
	}

	for(unsigned int i = 0; i < 16; i++)
	{
		temp.m_matrix[i] -= other.m_matrix[i];
	}

	return temp;
}

void Matrix44::operator=(const Matrix44& other)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		m_matrix[i] = other.m_matrix[i];
	}
}

bool Matrix44::operator==(const Matrix44& other)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		if(m_matrix[i] != other.m_matrix[i])
			return false;
	}
	return true;
}

Vector3 Matrix44::TransformPoint(const Vector3& vector )
{
	Vector3 temp = vector;
	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	temp.x = m_matrix[0] * x + m_matrix[1] * y + m_matrix[2] * z + m_matrix[3];
	temp.y = m_matrix[4] * x + m_matrix[5] * y + m_matrix[6] * z + m_matrix[7];
	temp.z = m_matrix[8] * x + m_matrix[9] * y + m_matrix[10] * z + m_matrix[11];;

	return temp;
}

Vector3 Matrix44::TransformDirection(const Vector3& vector)
{
	Vector3 temp = vector;
	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	temp.x = m_matrix[0] * x + m_matrix[1] * y + m_matrix[2] * z;
	temp.y = m_matrix[4] * x + m_matrix[5] * y + m_matrix[6] * z;
	temp.z = m_matrix[8] * x + m_matrix[9] * y + m_matrix[10] * z;

	return temp;
}

Matrix44 Matrix44::operator*( const Matrix44& matrix )
{
	Matrix44 result = this->TransformMatrix( matrix );
	return result;
}

Matrix44 Matrix44::TransformMatrix(const Matrix44& matrix)
{
	Matrix44 result;
	float lhs[16];
	float rhs[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		rhs[i] = matrix.m_matrix[i];
		lhs[i] = m_matrix[i];
	}

 	result.m_matrix[0] = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8] * rhs[2] + lhs[12] * rhs[3];
	result.m_matrix[1] = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9] * rhs[2] + lhs[13] * rhs[3];
	result.m_matrix[2] = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	result.m_matrix[3] = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	result.m_matrix[4] = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8] * rhs[6] + lhs[12] * rhs[7];
	result.m_matrix[5] = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9] * rhs[6] + lhs[13] * rhs[7];
	result.m_matrix[6] = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	result.m_matrix[7] = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	result.m_matrix[8] = lhs[0] * rhs[8] + lhs[4] * rhs[9] + lhs[8] * rhs[10] + lhs[12] * rhs[11];
	result.m_matrix[9] = lhs[1] * rhs[8] + lhs[5] * rhs[9] + lhs[9] * rhs[10] + lhs[13] * rhs[11];
	result.m_matrix[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] + lhs[10] * rhs[10] + lhs[14] * rhs[11];
	result.m_matrix[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] + lhs[11] * rhs[10] + lhs[15] * rhs[11];

	result.m_matrix[12] = lhs[0] * rhs[12] + lhs[4] * rhs[13] + lhs[8] * rhs[14] + lhs[12] * rhs[15];
	result.m_matrix[13] = lhs[1] * rhs[12] + lhs[5] * rhs[13] + lhs[9] * rhs[14] + lhs[13] * rhs[15];
	result.m_matrix[14] = lhs[2] * rhs[12] + lhs[6] * rhs[13] + lhs[10] * rhs[14] + lhs[14] * rhs[15];
	result.m_matrix[15] = lhs[3] * rhs[12] + lhs[7] * rhs[13] + lhs[11] * rhs[14] + lhs[15] * rhs[15];

	return result;
}

Matrix44 Matrix44::CreatePerspectiveMatrix(float fovY,float aspect,float zNear,float zFar)
{
	float f = 1.f / ( tan( MathUtilities::DegToRad( fovY * 0.5f ) ) );
	Matrix44 perspectiveMatrix;

	perspectiveMatrix.m_matrix[0] = f / aspect;
	perspectiveMatrix.m_matrix[5] = f;
	perspectiveMatrix.m_matrix[10] = ( zFar + zNear ) / ( zNear - zFar );
	perspectiveMatrix.m_matrix[11] = -1.f;
	perspectiveMatrix.m_matrix[14] = ( 2 * zFar * zNear ) / ( zNear - zFar );
	perspectiveMatrix.m_matrix[15] = 0.f;

	return perspectiveMatrix;
}

Matrix44 Matrix44::CreateOrthoMatrix(float left, float right, float bottom, float top, float near, float far)
{
	Matrix44 orthoMatrix;

	float tx = ( right  + left ) / ( right - left );
	float ty = ( top + bottom ) / ( top - bottom );
	float tz = ( far + near ) / ( far - near );

	orthoMatrix.m_matrix[0] = 2.f / ( right - left );
	orthoMatrix.m_matrix[5] = 2.f / ( top - bottom );
	orthoMatrix.m_matrix[10] = -2.f / ( far - near );
	orthoMatrix.m_matrix[12] = -tx;
	orthoMatrix.m_matrix[13] = -ty;
	orthoMatrix.m_matrix[14] = -tz;
	orthoMatrix.m_matrix[15] = 1.f;

	return orthoMatrix;
}

Matrix44 Matrix44::CancelTranslation(const Matrix44& matrix)
{
	Matrix44 temp;
	temp = matrix;
	temp.m_matrix[12] = 0.f;
	temp.m_matrix[13] = 0.f;
	temp.m_matrix[14] = 0.f;
	temp.m_matrix[15] = 1.f;
	return temp;
}

Matrix44 Matrix44::InvertMatrix( const Matrix44& matrix )
{
 	Matrix44 ret;

	float a00 = matrix.m_matrix[0], a01 = matrix.m_matrix[1], a02 = matrix.m_matrix[2], a03 = matrix.m_matrix[3],
		  a10 = matrix.m_matrix[4], a11 = matrix.m_matrix[5], a12 = matrix.m_matrix[6], a13 = matrix.m_matrix[7],
		  a20 = matrix.m_matrix[8], a21 = matrix.m_matrix[9], a22 = matrix.m_matrix[10], a23 = matrix.m_matrix[11],
		  a30 = matrix.m_matrix[12], a31 = matrix.m_matrix[13], a32 = matrix.m_matrix[14], a33 = matrix.m_matrix[15];

	float b00 = a00 * a11 - a01 * a10;
	float b01 = a00 * a12 - a02 * a10;
	float b02 = a00 * a13 - a03 * a10;
	float b03 = a01 * a12 - a02 * a11;
	float b04 = a01 * a13 - a03 * a11;
	float b05 = a02 * a13 - a03 * a12;
	float b06 = a20 * a31 - a21 * a30;
	float b07 = a20 * a32 - a22 * a30;
	float b08 = a20 * a33 - a23 * a30;
	float b09 = a21 * a32 - a22 * a31;
	float b10 = a21 * a33 - a23 * a31;
	float b11 = a22 * a33 - a23 * a32;

	float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

	if( !det )
		return nullptr;

	det = 1.f / det;

	ret.m_matrix[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
	ret.m_matrix[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
	ret.m_matrix[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
	ret.m_matrix[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
	ret.m_matrix[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
	ret.m_matrix[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
	ret.m_matrix[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
	ret.m_matrix[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
	ret.m_matrix[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
	ret.m_matrix[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
	ret.m_matrix[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
	ret.m_matrix[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
	ret.m_matrix[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
	ret.m_matrix[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
	ret.m_matrix[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
	ret.m_matrix[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;

	return ret;
}

Matrix44 Matrix44::TransposeMatrix( const Matrix44& matrix )
{
	Matrix44 ret;

	ret.m_matrix[0]  = matrix.m_matrix[0];
	ret.m_matrix[1]  = matrix.m_matrix[4];
	ret.m_matrix[2]  = matrix.m_matrix[8];
	ret.m_matrix[3]  = matrix.m_matrix[12];
	ret.m_matrix[4]  = matrix.m_matrix[1];
	ret.m_matrix[5]  = matrix.m_matrix[5];
	ret.m_matrix[6]  = matrix.m_matrix[9];
	ret.m_matrix[7]  = matrix.m_matrix[13];
	ret.m_matrix[8]  = matrix.m_matrix[2];
	ret.m_matrix[9]  = matrix.m_matrix[6];
	ret.m_matrix[10] = matrix.m_matrix[10];
	ret.m_matrix[11] = matrix.m_matrix[14];
	ret.m_matrix[12] = matrix.m_matrix[3];
	ret.m_matrix[13] = matrix.m_matrix[7];
	ret.m_matrix[14] = matrix.m_matrix[11];
	ret.m_matrix[15] = matrix.m_matrix[15];
								
	return ret;
}

Matrix44 Matrix44::CreateLookAtMatrix(const Vector3& cameraPos, const Vector3& target, const Vector3& up)
{
 	Vector3 forwardVector = ( cameraPos - target ).Normalize();
 	Vector3 upDirection = up;
 	upDirection = upDirection.Normalize();
 
 	Vector3 rightVector = upDirection.CrossProduct( forwardVector ).Normalize();
 	Vector3 upVector = forwardVector.CrossProduct( rightVector  ).Normalize();

	Matrix44 viewMatrix;

	viewMatrix.m_matrix[0] = rightVector.x;
	viewMatrix.m_matrix[4] = rightVector.y;
	viewMatrix.m_matrix[8] = rightVector.z;

	viewMatrix.m_matrix[1] = upVector.x;
	viewMatrix.m_matrix[5] = upVector.y;
	viewMatrix.m_matrix[9] = upVector.z;

	viewMatrix.m_matrix[2] = forwardVector.x;
	viewMatrix.m_matrix[6] = forwardVector.y;
	viewMatrix.m_matrix[10] = forwardVector.z;;
	
	viewMatrix.m_matrix[12] = -rightVector.DotProduct( cameraPos );
	viewMatrix.m_matrix[13] = -upVector.DotProduct( cameraPos );
	viewMatrix.m_matrix[14] = -forwardVector.DotProduct( cameraPos );

	return viewMatrix;
}
