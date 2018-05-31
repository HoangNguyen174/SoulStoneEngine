#include "GraphicText.hpp"

OpenGLShaderProgram*	GraphicText::m_defaultShaderProgram = nullptr;
BitmapFont*				GraphicText::m_defaultFont = nullptr;
Matrix44				GraphicText::m_3dFontRotateMatrix;
Matrix44				GraphicText::m_defaultOrthoMatrix;

GraphicText::GraphicText(const Vector3& bottomLeft, const Vector4& rotateAxisAndAngleDeg, const Vector3& scale, const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const Camera* camera, const std::string& myStr )
{
	m_bottomLeft = bottomLeft;
	m_rotateAxisAndAngleDeg = rotateAxisAndAngleDeg;
	m_scale = scale;
	m_color = color;
	m_cellHeight = cellHeight;
	m_content = myStr;
	m_camera = camera;
	m_vboID = 0;
	m_isVboDirty = true;
	m_contentLength = 0.f;

	if( m_defaultFont == nullptr )
		m_defaultFont = new BitmapFont("./Data/Font/MainFont_EN.FontDef.xml","./Data/Font/MainFont_EN00.png" );

	if( m_defaultShaderProgram == nullptr )
		m_defaultShaderProgram = new OpenGLShaderProgram( "Default Font Shader", "./Data/Shader/FontVertexShader.glsl","./Data/Shader/FontFragmentShader.glsl" );

	const Camera3D* camera3D = dynamic_cast<const Camera3D*>( camera );

	if( camera3D )
		m_3dFontRotateMatrix = Matrix44::CreateRotationAboutAxisDegrees( Vector3( 1.f, 0.f, 0.f), 90.f );
	
	m_defaultOrthoMatrix = Matrix44::CreateOrthoMatrix( 0.f, WINDOW_PHYSICAL_WIDTH, 0.f, WINDOW_PHYSICAL_HEIGHT, 0.f, 1.f );

	if( font == nullptr )
		m_font = m_defaultFont;
	else
		m_font = font;

	if( shader == nullptr )
		m_shaderProgram = m_defaultShaderProgram;
	else
		m_shaderProgram = shader;

	CreateVertices();
}

GraphicText::~GraphicText()
{
	delete m_defaultFont;
	delete m_defaultShaderProgram;
	glDeleteBuffers( 1, &m_vboID );
}

void GraphicText::CreateVBO()
{
	if( m_vboID == 0 )
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboID );
	
	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

	m_isVboDirty = false;
}

void GraphicText::CreateVertices()
{
	m_vertices.clear();

	SimpleVertex3D tempVertex;
	Vector2 texCoordMin;
	Vector2 texCoordMax;
	int asciiValue = 0;
	int index = 0;
	float ttfAWidth = 0.f;
	float ttfBWidth = 0.f;
	float ttfCWidth = 0.f;
	Vector3 positionTodraw = m_bottomLeft;

	for( unsigned int i = 0; i < m_content.size();i++ )
	{
		asciiValue = m_content[i];
		index = asciiValue - 32;

		texCoordMin = m_font->m_bitmapFontGlypdDataList[index].texCoordMins;
		texCoordMax = m_font->m_bitmapFontGlypdDataList[index].texCoordMax;
		ttfAWidth   = m_font->m_bitmapFontGlypdDataList[index].ttfA * m_cellHeight;
		ttfBWidth   = m_font->m_bitmapFontGlypdDataList[index].ttfB * m_cellHeight;
		ttfCWidth   = m_font->m_bitmapFontGlypdDataList[index].ttfC * m_cellHeight;

		positionTodraw.x += ttfAWidth;

		tempVertex.m_color = m_color;

		//bottom left corner
		tempVertex.m_position = positionTodraw;
		tempVertex.m_texCoords = Vector2(texCoordMin.x, texCoordMax.y);
		m_vertices.push_back(tempVertex);

		//bottom right corner
		tempVertex.m_position = Vector3(positionTodraw.x + ttfBWidth, positionTodraw.y, positionTodraw.z );
		tempVertex.m_texCoords = texCoordMax;
		m_vertices.push_back(tempVertex);

		//top right corner
		tempVertex.m_position = Vector3(positionTodraw.x + ttfBWidth, positionTodraw.y + m_cellHeight, positionTodraw.z );
		tempVertex.m_texCoords = Vector2(texCoordMax.x,texCoordMin.y);
		m_vertices.push_back(tempVertex);

		//top left corner
		tempVertex.m_position = Vector3(positionTodraw.x, positionTodraw.y + m_cellHeight, positionTodraw.z );
		tempVertex.m_texCoords = texCoordMin;
		m_vertices.push_back(tempVertex);

		positionTodraw.x += ( ttfBWidth + ttfCWidth );
		m_contentLength += ( ttfAWidth + ttfBWidth + ttfCWidth );
	}
}

void GraphicText::CalculateModelMatrix()
{
	Matrix44 translation;
	Matrix44 rotation;
	Matrix44 scaleMatrix;
	scaleMatrix = Matrix44::CreateNonUniformScaleMatrix( m_scale );
	rotation = Matrix44::CreateRotationAboutAxisDegrees( Vector3( m_rotateAxisAndAngleDeg.x, m_rotateAxisAndAngleDeg.y, m_rotateAxisAndAngleDeg.z ), m_rotateAxisAndAngleDeg.w );
	translation = Matrix44::CreateTranslationMatrix( m_bottomLeft );
	rotation = rotation * m_3dFontRotateMatrix;
	m_modelMatrix = translation * rotation * scaleMatrix;
}

void GraphicText::RenderVBO()
{
	GraphicManager::s_render->Enable( GL_TEXTURE_2D );
	GraphicManager::s_render->Disable( GL_DEPTH_TEST );

	GraphicManager::s_render->Enable (GL_BLEND);
	GraphicManager::s_render->BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shaderProgram->UseShaderProgram();

	Matrix44 modelMatrix;

	const Camera2D* camera2D = dynamic_cast<const Camera2D*>( m_camera );

	if( camera2D )
	{
		m_shaderProgram->SetMat4UniformValue( "u_projectionMatrix", m_camera->m_projectionMatrix );
		m_shaderProgram->SetMat4UniformValue( "u_viewMatrix", m_camera->m_viewMatrix );
		m_shaderProgram->SetMat4UniformValue( "u_modelMatrix", modelMatrix );
	}
	else
	{
		GraphicManager::s_render->Enable( GL_DEPTH_TEST );
		GraphicManager::s_render->Disable( GL_CULL_FACE );
		CalculateModelMatrix();
		m_shaderProgram->SetMat4UniformValue( "u_projectionMatrix", m_camera->m_projectionMatrix );
		m_shaderProgram->SetMat4UniformValue( "u_viewMatrix", m_camera->m_viewMatrix );
		m_shaderProgram->SetMat4UniformValue( "u_modelMatrix", m_modelMatrix );
	}

	m_shaderProgram->SetIntUniformValue( "u_fontTexture", 0 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, m_font->m_fontTextureSheet->m_openglTextureID );

	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_position ) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_color ) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_texCoords ) );

	GraphicManager::s_render->DrawArray( GL_QUADS ,0 , m_vertices.size() );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, 0 );
 
 	m_shaderProgram->DisableShaderProgram();
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Enable( GL_CULL_FACE );
	GraphicManager::s_render->Disable( GL_BLEND );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void GraphicText::Render()
{
	if( m_isVboDirty )
	{
		CreateVertices();
		CreateVBO();
	}

	RenderVBO();
}
