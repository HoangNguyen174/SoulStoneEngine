#include "IcoSphere.hpp"
#include "../Render/GraphicManager.hpp"

IcoSphere::IcoSphere( const Vector3& center, float radius, int recursionLevel )
{
	m_center = center;
	m_radius = radius;
	m_recursionLevel = recursionLevel;
	m_vertexIndex = 0;
	m_vboID = 0;
	m_iboID = 0;
	m_isVboDirty = true;
	GenerateVertices();
}

IcoSphere::~IcoSphere()
{

}

int IcoSphere::AddVertexPosition( const Vector3& vertexPosition )
{
	SimpleVertex3D vert;
	Vector3	pos = vertexPosition;

	pos = pos.Normalize();

	vert.m_position = pos;
	vert.m_color = RGBColor::White();

	m_vertices.push_back( vert );

	return m_vertexIndex++;
}

int IcoSphere::GetMiddlePoint( int p1Index, int p2Index )
{
	long long smallerIndex = MathUtilities::Min2( p1Index, p2Index );
	long long greaterIndex = MathUtilities::Max2i( p1Index, p2Index );
	long long key = ( smallerIndex << 32 ) + greaterIndex;

	int ret;

	std::map< long long, int >::iterator iter;
	iter = m_middlePointIndexCache.find( key );

	if( iter != m_middlePointIndexCache.end() )
	{
		ret = iter->second;
		return ret;
	}

	Vector3 pos1 = m_vertices[p1Index].m_position;
	Vector3 pos2 = m_vertices[p2Index].m_position;
	Vector3 middle = ( pos1 + pos2 ) * 0.5f;

	int i = AddVertexPosition( middle );

	m_middlePointIndexCache[ key ] = i;

	return i;
}

void IcoSphere::GenerateVertices()
{
	float t = ( 1.f + sqrt(5.f) ) * 0.5f;

	AddVertexPosition( Vector3( -1, t, 0 ) );
	AddVertexPosition( Vector3(  1, t, 0 ) );
	AddVertexPosition( Vector3( -1,-t, 0 ) );
	AddVertexPosition( Vector3(  1,-t, 0 ) );

	AddVertexPosition( Vector3( 0,-1, t ) );
	AddVertexPosition( Vector3( 0, 1, t ) );
	AddVertexPosition( Vector3( 0,-1,-t ) );
	AddVertexPosition( Vector3( 0, 1,-t ) );

	AddVertexPosition( Vector3(  t, 0,-1 ) );
	AddVertexPosition( Vector3(  t, 0, 1 ) );
	AddVertexPosition( Vector3( -t, 0,-1 ) );
	AddVertexPosition( Vector3( -t, 0, 1 ) );

	m_faces.push_back( TriangleIndices( 0, 11, 5 ) );
	m_faces.push_back( TriangleIndices( 0, 5, 1 ) );
	m_faces.push_back( TriangleIndices( 0, 1, 7 ) );
	m_faces.push_back( TriangleIndices( 0, 7, 10 ) );
	m_faces.push_back( TriangleIndices( 0, 10, 11 ) );

	m_faces.push_back( TriangleIndices( 1, 5, 9 ) );
	m_faces.push_back( TriangleIndices( 5, 11, 4 ) );
	m_faces.push_back( TriangleIndices( 11, 10, 2 ) );
	m_faces.push_back( TriangleIndices( 10, 7, 6 ) );
	m_faces.push_back( TriangleIndices( 7, 1, 8 ) );

	m_faces.push_back( TriangleIndices( 3, 9, 4 ) );
	m_faces.push_back( TriangleIndices( 3, 4, 2 ) );
	m_faces.push_back( TriangleIndices( 3, 2, 6 ) );
	m_faces.push_back( TriangleIndices( 3, 6, 8 ) );
	m_faces.push_back( TriangleIndices( 3, 8, 9 ) );

	m_faces.push_back( TriangleIndices( 4, 9, 5 ) );
	m_faces.push_back( TriangleIndices( 2, 4, 11 ) );
	m_faces.push_back( TriangleIndices( 6, 2, 10 ) );
	m_faces.push_back( TriangleIndices( 8, 6, 7 ) );
	m_faces.push_back( TriangleIndices( 9, 8, 1 ) );

	m_recursionLevel = 1;

	for( int i = 0; i < m_recursionLevel; i++ )
	{
		Faces faces2;
		for( unsigned int j = 0; j < m_faces.size(); j++ )
		{
			TriangleIndices tri = m_faces[j];

			int a = GetMiddlePoint( tri.x, tri.y );
			int b = GetMiddlePoint( tri.y, tri.z );
			int c = GetMiddlePoint( tri.z, tri.x );

			faces2.push_back( TriangleIndices( tri.x, a, c ) );
			faces2.push_back( TriangleIndices( tri.y, b, a ) );
			faces2.push_back( TriangleIndices( tri.z, c, b ) );
			faces2.push_back( TriangleIndices( a, b, c ) );	
		}
		m_faces = faces2; 
	}
}

void IcoSphere::CreateVBO()
{
	if( m_vboID == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboID );
	}

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( SimpleVertex3D ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

	if( m_iboID == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_iboID );
	}

	GraphicManager::s_render->BindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iboID );
	GraphicManager::s_render->BufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( TriangleIndices ) * m_faces.size(), m_faces.data(), GL_STATIC_DRAW );

	//m_isVboDirty = false;
}

void IcoSphere::Render()
{
	if( m_isVboDirty )
		CreateVBO();

	RenderVBO();
}

void IcoSphere::RenderVBO()
{
	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_position) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( SimpleVertex3D ), (const GLvoid*) offsetof(SimpleVertex3D,m_color) );

	GraphicManager::s_render->BindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iboID );
	glDrawElements( GL_TRIANGLES,  m_faces.size() * 3, GL_UNSIGNED_INT, (void*)0 );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
}