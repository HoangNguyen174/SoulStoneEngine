#ifndef ICO_SPHERE_H
#define ICO_SPHERE_H
#include "../Utilities/Vector3i.hpp"
#include "../Render/GLRender.hpp"

#include <map>
#include <vector>

typedef Vector3i						TriangleIndices;
typedef std::vector<SimpleVertex3D>		Vertices;
typedef std::vector<TriangleIndices>	Faces;

class IcoSphere
{
	public:
		Vector3						m_center;
		unsigned int				m_recursionLevel;
		float						m_radius;

	private:
		Vertices					m_vertices;
		Faces						m_faces;
		std::map< long long, int >	m_middlePointIndexCache;
		unsigned int				m_vboID;
		unsigned int				m_iboID;
		bool						m_isVboDirty;
		unsigned int				m_vertexIndex;

	public:
		IcoSphere( const Vector3& center, float radius, int recursionLevel );
		~IcoSphere();
		void Render();

	private:
		int AddVertexPosition( const Vector3& vertexPosition );
		int GetMiddlePoint( int p1Index, int p2Index );
		void GenerateVertices();
		void CreateVBO();
		void RenderVBO();
};


#endif