#include "Camera3D.hpp"
#include "../Render/GraphicManager.hpp"

Camera3D::Camera3D( const std::string& name ) : Camera( name )
{
	m_pitchDegreesAboutY = 0.f;
	m_yawDegreesAboutZ = 0.f;
	m_rollDegreesAboutX = 0.f;
	m_movementSpeed = DEFAULT_CAMERA_MOVESPEED;
	GraphicManager::s_3DCameraRegistry[ name ] = this;
}

Camera3D::~Camera3D()
{
	
}

void Camera3D::SetupPerspectiveProjection( float fov, float aspectRatio, float zNear, float zFar )
{
	m_projectionMatrix = Matrix44::CreatePerspectiveMatrix( fov, aspectRatio, zNear, zFar );
}

void Camera3D::Update( float elapsedTime )
{
	UpdateCameraFromKeyboardAndMouse( elapsedTime );
}

void Camera3D::ApplyCameraTranform()
{
	MatrixStack stack;

	stack.LoadIdentity();

	stack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3( 1.f, 0.f, 0.f), -90.f ) );
	stack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3( 0.f, 0.f, 1.f ), 90.f ) );

	stack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3( 1.f, 0.f, 0.f ), -m_rollDegreesAboutX ) );
	stack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3( 0.f, 1.f, 0.f ), -m_pitchDegreesAboutY ) );
	stack.PushMatrix( Matrix44::CreateRotationAboutAxisDegrees( Vector3( 0.f, 0.f, 1.f ), -m_yawDegreesAboutZ ) );
	
	stack.PushMatrix( Matrix44::CreateTranslationMatrix( Vector3( -m_cameraPosition.x, -m_cameraPosition.y, -m_cameraPosition.z ) ) );

	m_viewMatrix = stack.StackTop();
}

void Camera3D::UpdateCameraFromKeyboardAndMouse( float elapsedTime )
{
	const float degreesPerMouseDelta = 0.02f;
	Vector2 mouseDeltas = GetMouseSinceLastChecked();
	m_yawDegreesAboutZ -= ( degreesPerMouseDelta * mouseDeltas.x );

	m_pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y);
	m_pitchDegreesAboutY = MathUtilities::Clamp( -90.f, 90.f, m_pitchDegreesAboutY );

	float cameraYawRadians = MathUtilities::DegToRad( m_yawDegreesAboutZ );
	Vector3 cameraForwardXYVector( cos(cameraYawRadians), sin( cameraYawRadians ),0.f );

	m_forwardDirection = Vector3( cameraForwardXYVector.x * cos( MathUtilities::DegToRad( m_pitchDegreesAboutY ) ),
								  cameraForwardXYVector.y * cos( MathUtilities::DegToRad( m_pitchDegreesAboutY ) ),
								  -sin( MathUtilities::DegToRad( m_pitchDegreesAboutY ) ) );

	m_forwardDirection = m_forwardDirection.Normalize();

	Vector3 movementVector( 0.f,0.f,0.f );

	if( IsKeyDownKeyboard[ 'W' ] )
		movementVector += cameraForwardXYVector * m_movementSpeed;

	if( IsKeyDownKeyboard[ 'A' ] )
		movementVector += Vector3(-cameraForwardXYVector.y,cameraForwardXYVector.x,0.f) * m_movementSpeed;

	if( IsKeyDownKeyboard[ 'D' ] )
		movementVector += Vector3(cameraForwardXYVector.y,-cameraForwardXYVector.x,0.f) * m_movementSpeed;

	if( IsKeyDownKeyboard[ 'S' ] )
		movementVector += Vector3(-cameraForwardXYVector.x,-cameraForwardXYVector.y,0.f) * m_movementSpeed;

	if( IsKeyDownKeyboard[ 'E' ] )
		movementVector += Vector3(0.f,0.f, m_movementSpeed);

	if( IsKeyDownKeyboard[ 'C' ] )
		movementVector += Vector3(0.f,0.f, -m_movementSpeed);

	m_cameraPosition += movementVector * elapsedTime;
}

Matrix44 Camera3D::GetProjectionViewMatrix()
{
	m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix; 
	return m_projectionViewMatrix;
}

Vector2 Camera3D::GetMouseSinceLastChecked()
{
	POINT centerCursorPos = { WINDOW_PHYSICAL_WIDTH / 2, WINDOW_PHYSICAL_HEIGHT / 2 };
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vector2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vector2 mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}

Camera3D* Camera3D::Get3DCameraByName(const std::string& name)
{
	auto found = GraphicManager::s_3DCameraRegistry.find( name );
	if( found == GraphicManager::s_3DCameraRegistry.end() )
		return nullptr;
	else
		return GraphicManager::s_3DCameraRegistry[ name ];
}

Camera3D* Camera3D::CreateOrGetCameraByName( const std::string& name )
{
	if( Get3DCameraByName( name ) != nullptr )
	{
		return GraphicManager::s_3DCameraRegistry[ name ];
	}
	else
	{
		Camera3D* camera = new Camera3D( name );
		return camera;
	}
}

void Camera3D::LookAt(const Vector3& lookAtTarget, const Vector3& up)
{
	
}
