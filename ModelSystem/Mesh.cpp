#include "Mesh.hpp"

void Mesh::Render( OpenGLShaderProgram* shader )
{
	for( unsigned int i = 0; i < m_triBatList.size(); i++ )
	{
		m_triBatList[i]->Render( shader );
	}
}