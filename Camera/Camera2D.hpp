#ifndef CAMERA2D_H
#define CAMERA2D_H
#include "Camera.hpp"

class Camera2D : public Camera
{
	public:
		WorldCoords2D		m_center;
		float				m_viewportWidth;
		float				m_viewportHeight;

	public:
		void ApplyCameraTransform();
		void SetupOrthoProjection( float left, float right, float bottom, float top, float near, float far );
		void Update( float elapsdeTime );
		static Camera2D* Get2DCameraByName( const std::string& name );
		static Camera2D* CreateOrGetCameraByName( const std::string& name );
		Matrix44 GetProjectionViewMatrix();

	private:
		Camera2D( const std::string& name );
		Camera2D( const std::string& name, float width, float height );
		~Camera2D();
		void UpdateCameraFromKeyboard( float elapsedTime );
};


#endif