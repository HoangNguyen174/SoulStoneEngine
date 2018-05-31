#include "SkyBox.hpp"

SkyBox::SkyBox( float radius, float height, const Camera3D* camera, const Texture* texture )
{
	m_radius = radius;
	m_height = height;
	m_camera = camera;
	m_diffuseTexture = texture;
}

SkyBox::~SkyBox()
{

}

void SkyBox::CreateSkyBoxVertices()
{
	SimpleVertex3D vertex;

	vertex.m_color = RGBColor( 0.047f, 0.047f, 0.5f, 1.f );
	//south
	vertex.m_position = Vector3( -m_radius,   m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius,  -m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius,  -m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius,   m_radius, 0.f );
	m_vertices.push_back( vertex );

	//north
	vertex.m_position = Vector3(  m_radius,   m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius,   m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius,  -m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius,  -m_radius, m_height );
	m_vertices.push_back( vertex );

	//west
	vertex.m_position = Vector3(  m_radius,   m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius,   m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius,  -m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius,  -m_radius, m_height );
	m_vertices.push_back( vertex );

	//east
	vertex.m_position = Vector3( -m_radius, -m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius, -m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius, -m_radius, 0.f );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius, -m_radius, 0.f );
	m_vertices.push_back( vertex );

	//top
	vertex.m_position = Vector3( -m_radius,  m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius,  m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3(  m_radius, -m_radius, m_height );
	m_vertices.push_back( vertex );
	vertex.m_position = Vector3( -m_radius, -m_radius, m_height );
	m_vertices.push_back( vertex );
}

void SkyBox::CreateVBO()
{
	if( m_vboId == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboId );
	}

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboId );
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );
}

void SkyBox::RenderVBO()
{
	m_shaderProgram->UseShaderProgram();

	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboId );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_position ) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_color ) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), ( const GLvoid* ) offsetof( SimpleVertex3D,m_texCoords ) );

	GraphicManager::s_render->DrawArray( GL_QUADS , 0, m_vertices.size() );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, 0 );

	m_shaderProgram->DisableShaderProgram();
}

void SkyBox::Update( float elapsedTime )
{

}

void SkyBox::Render()
{
	GraphicManager::s_render->UseShaderProgram( 0 );

	float skyBoxTextWidth = 1.f / 4.f;
	float skyBoxTextHeight = 1.f / 3.f;

	GraphicManager::s_render->Disable( GL_DEPTH_TEST );
	GraphicManager::s_render->Disable( GL_CULL_FACE );

	glActiveTexture( GL_TEXTURE0 );
	GraphicManager::s_render->Enable( GL_TEXTURE_2D );
	GraphicManager::s_render->BindTexture( GL_TEXTURE_2D, m_diffuseTexture->m_openglTextureID );

	GraphicManager::s_render->PushMatrix();
	GraphicManager::s_render->Translatef( m_camera->m_cameraPosition.x, m_camera->m_cameraPosition.y, 0.f );
	GraphicManager::s_render->Scalef( 1.f, 1.f, 1.f);

	GraphicManager::s_render->Color4f( 1.f, 1.f, 1.f, 1.f );

	GraphicManager::s_render->BeginDraw( GL_QUADS );
	{
		//south
		GraphicManager::s_render->TexCoord2d(  4 * skyBoxTextWidth ,  skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius, m_radius, (int)m_height );
		GraphicManager::s_render->TexCoord2d( 3 * skyBoxTextWidth ,  skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius, -m_radius, (int)m_height );
		GraphicManager::s_render->TexCoord2d( 3 * skyBoxTextWidth ,  2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius,-m_radius, 0 );
		GraphicManager::s_render->TexCoord2d( 4 * skyBoxTextWidth ,  2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius,m_radius, 0 );

		//north
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth ,  skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( m_radius, m_radius,(int)m_height);
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth , 2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( m_radius, m_radius, 0 );
		GraphicManager::s_render->TexCoord2d( 2 * skyBoxTextWidth , 2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( m_radius,-m_radius, 0 );
		GraphicManager::s_render->TexCoord2d( 2 * skyBoxTextWidth ,  skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i(m_radius, -m_radius, (int)m_height); 

		//west
		GraphicManager::s_render->TexCoord2d(  0.f , 2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i(-m_radius, m_radius,0 );
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth , 2 * skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( m_radius, m_radius,0 );
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth , skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( m_radius, m_radius,(int)m_height);
		GraphicManager::s_render->TexCoord2d( 0 , skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius, m_radius,(int)m_height);

		//east
		GraphicManager::s_render->TexCoord2d( 3 * skyBoxTextWidth , skyBoxTextHeight );
		GraphicManager::s_render->Vertex3i( -m_radius,-m_radius,(int)m_height);
		GraphicManager::s_render->TexCoord2d( 2 * skyBoxTextWidth , 1 * skyBoxTextHeight);
		GraphicManager::s_render->Vertex3i( m_radius,-m_radius,(int)m_height);
		GraphicManager::s_render->TexCoord2d( 2 * skyBoxTextWidth , 2 * skyBoxTextHeight);
		GraphicManager::s_render->Vertex3i( m_radius,-m_radius,0 );
		GraphicManager::s_render->TexCoord2d( 3 * skyBoxTextWidth , 2 * skyBoxTextHeight);
		GraphicManager::s_render->Vertex3i( -m_radius,-m_radius,0 );

		//top
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth , 0.f);
		GraphicManager::s_render->Vertex3i( -m_radius, m_radius, (int)m_height);
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth , skyBoxTextHeight);
		GraphicManager::s_render->Vertex3i( m_radius, m_radius, (int)m_height);
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth + skyBoxTextWidth , skyBoxTextHeight);
		GraphicManager::s_render->Vertex3i( m_radius,-m_radius, (int)m_height);
		GraphicManager::s_render->TexCoord2d( skyBoxTextWidth + skyBoxTextWidth , 0.f);
		GraphicManager::s_render->Vertex3i(-m_radius,-m_radius, (int)m_height);

	}
	GraphicManager::s_render->EndDraw();

	GraphicManager::s_render->PopMatrix();
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
//	GraphicManager::s_render->Enable( GL_BLEND );
	GraphicManager::s_render->Disable( GL_TEXTURE_2D );

}