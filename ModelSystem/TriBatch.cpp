#include "TriBatch.hpp"
#include "Mesh.hpp"
#include "../Render/GraphicManager.hpp"

TriBatch::TriBatch()
{
	m_vboID = 0;
	m_isVBODirty = true;
}

void TriBatch::CreateVBO()
{
	if( m_vboID == 0 )
	{
		GraphicManager::s_render->GenerateBuffer( 1, &m_vboID );
	}

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID);
	GraphicManager::s_render->BufferData( GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexInLocalSpaceList.size(), m_vertexInLocalSpaceList.data(), GL_STATIC_DRAW );

	m_numVertex = m_vertexInLocalSpaceList.size();
	m_vertexInLocalSpaceList.clear();
	vector<Vertex3D>().swap( m_vertexInLocalSpaceList );
	m_isVBODirty = false;
}

void TriBatch::Render( OpenGLShaderProgram* shader )
{
	Matrix44 identity;

	m_material->SetUniformValuesToShaderProgram( shader );
	m_material->BindTexture();

	if( m_isVBODirty )
	{
		CreateVBO();
	}

	GraphicManager::s_render->Enable( GL_TEXTURE_2D );
	GraphicManager::s_render->Enable( GL_DEPTH_TEST );
	GraphicManager::s_render->Enable(GL_CULL_FACE);
	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TANGENT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BITANGENT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BONE_INDEX );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BONE_WEIGHT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_IS_STATIC );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, m_vboID );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_position) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_color) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_texCoords) );
	glVertexAttribPointer( VERTEX_ATTRIB_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_normal) );
	glVertexAttribPointer( VERTEX_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_tangent) );
	glVertexAttribPointer( VERTEX_ATTRIB_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_bitangent) );
	glVertexAttribPointer( VERTEX_ATTRIB_BONE_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_boneIndex) ) );
	glVertexAttribPointer( VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_boneWeight ) ) );
	glVertexAttribPointer( VERTEX_ATTRIB_IS_STATIC, 1, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_isStatic ) ) );

	GraphicManager::s_render->DrawArray( GL_TRIANGLES ,0, m_numVertex );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TANGENT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BITANGENT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BONE_INDEX );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BONE_WEIGHT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_IS_STATIC );

	GraphicManager::s_render->BindBuffer( GL_ARRAY_BUFFER, 0 );
	GraphicManager::s_render->Disable( GL_TEXTURE_2D );
	GraphicManager::s_render->Disable( GL_DEPTH_TEST );
}

void TriBatch::SetMaterial(const std::string& materialName)
{
	std::set<Material*>::iterator iter;
	for( iter = GraphicManager::s_render->s_materialRegisteredList.begin(); iter != GraphicManager::s_render->s_materialRegisteredList.end(); ++iter )
	{
		Material* mat = *iter;
		if( mat->m_materialName.compare( materialName ) == 0 )
		{
			m_material = mat;
		}
	}
}

void TriBatch::TransformToWorldSpace()
{
	m_vertexInWorldSpaceList.reserve( m_vertexInLocalSpaceList.size() );

	m_vertexInWorldSpaceList.clear();
	for( unsigned int i = 0; i < m_vertexInLocalSpaceList.size(); i++ )
	{
		Vertex3D vertex = m_vertexInLocalSpaceList[i];
		vertex.m_position = m_parentMesh->m_toWorldTransformation->TransformPoint( vertex.m_position );
		vertex.m_normal = m_parentMesh->m_toWorldTransformationNoTranslation->TransformDirection( vertex.m_normal );
		vertex.m_normal = vertex.m_normal.Normalize();
		vertex.m_tangent = m_parentMesh->m_toWorldTransformationNoTranslation->TransformDirection( vertex.m_tangent );
		vertex.m_bitangent = m_parentMesh->m_toWorldTransformationNoTranslation->TransformDirection( vertex.m_bitangent );
		m_vertexInWorldSpaceList.push_back( vertex );
	}
}
