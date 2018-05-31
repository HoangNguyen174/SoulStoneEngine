#ifndef TRI_BATCH_H
#define TRI_BATCH_H
#include <string>
#include "../Utilities/Matrix44.hpp"
#include "../Render/Material.hpp"
#include "../Utilities/GameCommon.hpp"

class Mesh;

class TriBatch
{
	public:
		Mesh*					m_parentMesh;
		Material*				m_material;
		GLuint					m_vboID;
		std::vector<Vertex3D>	m_vertexInLocalSpaceList;
		std::vector<Vertex3D>	m_vertexInWorldSpaceList;
		bool					m_isVBODirty;
		int						m_numVertex;
	
	public:
		TriBatch();
		void TransformToWorldSpace();
		void SetMaterial( const std::string& m_materialName );
		void Render( OpenGLShaderProgram* shader );

	private:
		void CreateVBO();

};
#endif