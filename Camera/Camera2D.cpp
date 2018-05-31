#include "Camera2D.hpp"
#include "../Render/GraphicManager.hpp"

Camera2D::Camera2D( const std::string& name ) : Camera( name )
{ 
	m_viewportHeight = WINDOW_VIRTUAL_HEIGHT; 
	m_viewportWidth = WINDOW_VIRTUAL_WIDTH; 
	m_center.x = m_viewportWidth * 0.5f;
	m_center.y = m_viewportHeight * 0.5f;
	GraphicManager::s_2DCameraRegistry[ name ] = this;
}

Camera2D::Camera2D( const std::string& name, float width, float height ) : Camera( name )
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	m_center.x = m_viewportWidth * 0.5f;
	m_center.y = m_viewportHeight * 0.5f;
	GraphicManager::s_2DCameraRegistry[ name ] = this;
}

Camera2D::~Camera2D()
{

}

void Camera2D::SetupOrthoProjection( float left, float right, float bottom, float top, float zNear, float zFar )
{
	m_projectionMatrix = Matrix44::CreateOrthoMatrix( left, right, bottom, top, zNear, zFar );
}

void Camera2D::Update( float elapsedTime )
{
	UpdateCameraFromKeyboard( elapsedTime );
}

void Camera2D::UpdateCameraFromKeyboard( float elapsedTime )
{
	if( IsKeyDownKeyboard[ 'W' ] )
	{
		m_center.y += MOUSE_MOVE_RATE * elapsedTime;
	}

	if( IsKeyDownKeyboard[ 'S' ] )
	{
		m_center.y -= MOUSE_MOVE_RATE * elapsedTime;
	}

	if( IsKeyDownKeyboard[ 'D' ] )
	{
		m_center.x += MOUSE_MOVE_RATE * elapsedTime;
	}

	if( IsKeyDownKeyboard[ 'A' ] )
	{
		m_center.x -= MOUSE_MOVE_RATE * elapsedTime;
	}

	if( IsKeyDownKeyboard[ 'R' ] && IsKeyDownKeyboard[ 'R' ] != IsKeyDownLastFrame[ 'R' ] )
	{
		m_viewportWidth *= ZOOM_FACTOR;
		m_viewportHeight *= ZOOM_FACTOR;
	}

	if( IsKeyDownKeyboard[ 'E' ] && IsKeyDownKeyboard[ 'E' ] != IsKeyDownLastFrame[ 'E' ] )
	{
		m_viewportWidth /= ZOOM_FACTOR;
		m_viewportHeight /= ZOOM_FACTOR;
	}
}

void Camera2D::ApplyCameraTransform()
{
	Vector3 translation( -( m_center.x - m_viewportWidth * 0.5f ), -( m_center.y - m_viewportHeight * 0.5f ), 0.f  );
	m_viewMatrix = Matrix44::CreateTranslationMatrix( translation );
	m_projectionMatrix = Matrix44::CreateOrthoMatrix( 0.f, m_viewportWidth, 0.f, m_viewportHeight, 0.f, 1.f );
}

Camera2D* Camera2D::Get2DCameraByName(const std::string& name)
{
	auto found = GraphicManager::s_2DCameraRegistry.find( name );
	if( found == GraphicManager::s_2DCameraRegistry.end() )
		return nullptr;
	else
		return GraphicManager::s_2DCameraRegistry[ name ];
}

Camera2D* Camera2D::CreateOrGetCameraByName( const std::string& name )
{
	if( Get2DCameraByName( name  ) != nullptr )
	{
		return GraphicManager::s_2DCameraRegistry[ name ];
	}
	else
	{
		Camera2D* camera = new Camera2D( name );
		return camera;
	}
}

Matrix44 Camera2D::GetProjectionViewMatrix()
{
	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix; 
	return m_projectionViewMatrix;
}
