#ifndef CAMERA_H
#define CAMERA_H
#include "../Utilities/GameCommon.hpp"
#include "../Utilities/Matrix44.hpp"

class Camera
{
	public:
		std::string			m_name;
		Matrix44			m_projectionMatrix;
		Matrix44			m_viewMatrix;
		Matrix44			m_projectionViewMatrix;

	public:
		Camera( const std::string& name ) : m_name(name) {};
		virtual ~Camera() {};
};

#endif