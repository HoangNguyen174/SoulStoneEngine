#ifndef SKY_BOX_H
#define SKY_BOX_H
#include "../Utilities/GameCommon.hpp"
#include "../Render/GraphicManager.hpp"

class SkyBox
{
	public:
		Vector3					m_bottomCenter;
		float					m_radius;
		float					m_height;
		const Texture*			m_diffuseTexture;
		OpenGLShaderProgram*	m_shaderProgram;

	private:
		std::vector<SimpleVertex3D> m_vertices;
		unsigned int				m_vboId;
		const Camera3D*				m_camera;

	public:
		SkyBox( float radius, float height, const Camera3D* camera, const Texture* texture );
		~SkyBox();
		void Update( float elapsedTime );
		void Render();

	private:
		void CreateVBO();
		void RenderVBO();
		void CreateSkyBoxVertices();
};

#endif