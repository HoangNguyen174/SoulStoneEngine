#ifndef CAMERA3D_H
#define CAMERA3D_H

#include "../Utilities/MatrixStack.hpp"
#include "Camera.hpp"

const float DEFAULT_CAMERA_MOVESPEED = 10.f;

class Camera3D : public Camera
{
	public:
		Vector3			m_cameraPosition;
		Vector3			m_forwardDirection;
		float			m_yawDegreesAboutZ;
		float			m_pitchDegreesAboutY;
		float			m_rollDegreesAboutX;
		float			m_movementSpeed;

	public:
		Matrix44 GetProjectionViewMatrix();
		void ApplyCameraTranform();
		void SetupPerspectiveProjection( float fov, float aspectRatio, float zNear, float zFar );
		void Update( float elapsdeTime );
		static Camera3D* CreateOrGetCameraByName( const std::string& name );
		void LookAt( const Vector3& lookAtTarget, const Vector3& up );

	private:
		Camera3D( const std::string& name );
		~Camera3D();
		void UpdateCameraFromKeyboardAndMouse( float elapsedTime );
		Vector2 GetMouseSinceLastChecked();
		static Camera3D* Get3DCameraByName( const std::string& name );
};

#endif