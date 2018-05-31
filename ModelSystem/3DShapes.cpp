#include "3DShapes.hpp"
#include "../Render/GraphicManager.hpp"

Sphere::Sphere( float radius, int lats, int longs )
{
	m_radius = radius;
	m_lattitude = lats;
	m_longtitude = longs;
	m_isVboDirty = true;
	m_vboID = 0;
	GenerateSphereVertices();
}

Sphere::Sphere()
{
	m_radius = 1;
	m_lattitude = 10;
	m_longtitude = 10;
	m_isVboDirty = true;
	m_vboID = 0;
	GenerateSphereVertices();
}

void Sphere::GenerateSphereVertices()
{
	SimpleVertex3D vertex;

	vertex.m_color = RGBColor( 1.f, 1.f, 1.f, 1.f);

	for( int i = 0; i <= m_lattitude; i++ ) 
	{
		float lat0 = 3.14f * ( -0.5f + (float) (i - 1) / m_lattitude );
		float z0  = sin(lat0);
		float zr0 =  cos(lat0);

		float lat1 = 3.14f * ( -0.5f + (float) i / m_lattitude );
		float z1 = sin(lat1);
		float zr1 = cos(lat1);

		for( int j = 0; j <= m_longtitude; j++ ) 
		{
			float lng = 2 * 3.14f * (float) (j - 1) / m_longtitude;
			float x = cos(lng);
			float y = sin(lng);

			vertex.m_texCoords = Vector2(  x * zr1, y * zr1 );
			vertex.m_normal = Vector3( x * zr1, y * zr1, z1 );
			vertex.m_position = Vector3( x * zr1, y * zr1, z1 );
			m_vertices.push_back(vertex);

			vertex.m_texCoords = Vector2(  x * zr0, y * zr0 );
			vertex.m_normal = Vector3( x * zr0, y * zr0, z0 );
			vertex.m_position = Vector3( x * zr0, y * zr0, z0 );
			m_vertices.push_back(vertex);
		}
	}
}

void Sphere::CreateVBO()
{
	if( m_vboID == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboID );
	}

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

	m_isVboDirty = false;
}

void Sphere::Render()
{
	if( m_isVboDirty )
		CreateVBO();

	RenderVBO();
}

void Sphere::RenderVBO()
{
	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_NORMALS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_position) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_color) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_texCoords) );
	glVertexAttribPointer( VERTEX_ATTRIB_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_normal) );

	GraphicManager::s_render->DrawArray( GL_QUAD_STRIP , 0, m_vertices.size() );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
}


ScreenRectangle::~ScreenRectangle()
{
	glDeleteBuffers( 1, &m_vboID );
}

ScreenRectangle::ScreenRectangle()
{
	SimpleVertex3D vert;

	vert.m_position = Vector3( -1.f, -1.f, 0.f );
	vert.m_texCoords = Vector2( 0.f, 0.f );
	m_vertices.push_back( vert );

	vert.m_position = Vector3(  1.f, -1.f, 0.f );
	vert.m_texCoords = Vector2( 1.f, 0.f );
	m_vertices.push_back( vert );

	vert.m_position = Vector3(  1.f, 1.f, 0.f );
	vert.m_texCoords = Vector2( 1.f, 1.f );
	m_vertices.push_back( vert );

	vert.m_position = Vector3( -1.f, 1.f, 0.f );
	vert.m_texCoords = Vector2( 0.f, 1.f );
	m_vertices.push_back( vert );
}

void ScreenRectangle::CreateVBO()
{
	if( m_vboID == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboID );
	}

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

	m_isVboDirty = false;
}

void ScreenRectangle::RenderVBO()
{
	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_NORMALS );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_position) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_color) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_texCoords) );
	glVertexAttribPointer( VERTEX_ATTRIB_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_normal) );

	GraphicManager::s_render->DrawArray( GL_QUADS , 0, m_vertices.size() );
	
	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
}

void ScreenRectangle::Render()
{
	if( m_isVboDirty )
		CreateVBO();

	RenderVBO();
}


